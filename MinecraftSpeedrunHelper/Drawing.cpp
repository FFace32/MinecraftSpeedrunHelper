#include "Drawing.h"
#include <Windows.h>
#include <gl/GL.h>
#include "SDK/net/minecraft/client/Minecraft.h"
#include "SDK/net/minecraft/client/Timer.h"
#include "SDK/net/minecraft/client/renderer/GameRenderer.h"
#include "SDK/net/minecraft/client/Camera.h"
#include "Math/Math.h"

using namespace Drawing;

Point::Point( int x, int y ) :
	x( x ),
	y( y )
{ }

Color::Color( uint8_t r, uint8_t g, uint8_t b, uint8_t a /*= 255*/ ) :
	r( r ),
	g( g ),
	b( b ),
	a( a )
{ }

Context::Context( int Width, int Height, net::minecraft::client::Minecraft* Minecraft, float LineWidth ) :
	ScreenCenter( Width / 2, Height / 2 ),
	LineWidth( LineWidth )
{
	const auto GameRenderer = Minecraft->gameRenderer();
	const auto MainCamera = GameRenderer->getMainCamera();
	Camera.Position = MainCamera->position();
	Camera.Forwards = MainCamera->forwards();
	Camera.Up = MainCamera->up();
	Camera.Left = MainCamera->left();
	Camera.FovY = Math::tand( GameRenderer->getFov( MainCamera, Minecraft->pause() ? Minecraft->pausePartialTick() : Minecraft->timer()->partialTick(), true ) * 0.5 );
	Camera.FovX = Camera.FovY * ( static_cast<double>( Width ) / static_cast<double>( Height ) );
}

Context Drawing::Begin( net::minecraft::client::Minecraft* Minecraft )
{
	glPushClientAttrib( GL_CLIENT_ALL_ATTRIB_BITS );
	glPushAttrib( GL_ALL_ATTRIB_BITS );
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
	glDisable( GL_SCISSOR_TEST );
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_INDEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_EDGE_FLAG_ARRAY );
	glMatrixMode( GL_TEXTURE );
	glPushMatrix();
	glLoadIdentity();
	glPolygonMode( GL_FRONT, GL_FILL );
	glPolygonMode( GL_BACK, GL_FILL );
	glDisable( GL_LIGHTING );
	glDisable( GL_FOG );
	glDisable( GL_CULL_FACE );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_ALPHA_TEST );
	glDisable( GL_TEXTURE_GEN_S );
	glDisable( GL_TEXTURE_GEN_T );
	glDisable( GL_TEXTURE_GEN_R );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glShadeModel( GL_SMOOTH );

	GLint ViewPort[4];
	glGetIntegerv( GL_VIEWPORT, ViewPort );
	
	glMatrixMode( GL_PROJECTION );
	glOrtho( 0.0, static_cast<GLdouble>( ViewPort[2] ), static_cast<GLdouble>( ViewPort[3] ), 0.0, 0.0, 1.0 );

	GLfloat LineWidth;
	glGetFloatv( GL_LINE_WIDTH, &LineWidth );
	glLineWidth( 1.f );
	
	return Context( ViewPort[2], ViewPort[3], Minecraft, LineWidth );
}

void Drawing::End( const Context& Context )
{
	glLineWidth( Context.LineWidth );
	
	glMatrixMode( GL_TEXTURE );
	glPopMatrix();
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
	glPopAttrib();
	glPopClientAttrib();
}

bool Drawing::WorldToScreen( const Context& Context, const Math::Vector& World, Point& Screen )
{
	const auto Delta = World - Context.Camera.Position;
	
	const auto z = Delta.Dot( Context.Camera.Forwards );
	if ( z < 0.01 )
		return false;
	
	const auto x = Delta.Dot( Context.Camera.Left );
	const auto y = Delta.Dot( Context.Camera.Up );

	Screen.x = static_cast<int>( Context.ScreenCenter.x - Context.ScreenCenter.x / z / Context.Camera.FovX * x );
	Screen.y = static_cast<int>( Context.ScreenCenter.y - Context.ScreenCenter.y / z / Context.Camera.FovY * y );

	return true;
}

void Drawing::DrawLine( const Color& Color, const Point& Location0, const Point& Location1 )
{
	DrawLine( Color, Location0.x, Location0.y, Location1.x, Location1.y );
}

void Drawing::DrawLine( const Color& Color, int x0, int y0, int x1, int y1 )
{
	glColor4ub( Color.r, Color.g, Color.b, Color.a );

	glBegin( GL_LINES );
	glVertex2i( x0, y0 );
	glVertex2i( x1, y1 );
	glEnd();
}

void DrawLineInternal( const Point& Location0, const Point& Location1 )
{
	glVertex2i( Location0.x, Location0.y );
	glVertex2i( Location1.x, Location1.y );
}

void Drawing::DrawBox( const Context& Context, const Math::AABB& AABB, const Color& Color )
{
	DrawBox( Context, Math::Vector::ZERO(), Math::Vector::ZERO(), AABB.Mins, AABB.Maxs, Color );
}

// IIRC I got all the following functions from the Source engine
static void AngleMatrix( const Math::Vector& angles, Math::Matrix3x4& matrix )
{
	double sr, sp, sy, cr, cp, cy;
	Math::SinCosD( angles.y, sp, cp );
	Math::SinCosD( angles.x, sy, cy );
	Math::SinCosD( angles.z, sr, cr );

	// matrix = (YAW * PITCH) * ROLL
	matrix[0][0] = cp * cy;
	matrix[1][0] = cp * sy;
	matrix[2][0] = -sp;

	auto crcy = cr * cy;
	auto crsy = cr * sy;
	auto srcy = sr * cy;
	auto srsy = sr * sy;
	matrix[0][1] = sp * srcy - crsy;
	matrix[1][1] = sp * srsy + crcy;
	matrix[2][1] = sr * cp;

	matrix[0][2] = ( sp * crcy + srsy );
	matrix[1][2] = ( sp * crsy - srcy );
	matrix[2][2] = cr * cp;

	matrix[0][3] = 0.f;
	matrix[1][3] = 0.f;
	matrix[2][3] = 0.f;
}

void VectorRotate( const Math::Vector& in1, const Math::Matrix3x4& in2, Math::Vector& out )
{
	out.x = in1.Dot( in2[0] );
	out.y = in1.Dot( in2[1] );
	out.z = in1.Dot( in2[2] );
}

static void GenerateBoxVertices( const Math::Vector& vOrigin, const Math::Vector& angles, const Math::Vector& vMins, const Math::Vector& vMaxs, Math::Vector pVerts[8] )
{
	// Build a rotation matrix from orientation
	Math::Matrix3x4 fRotateMatrix;
	AngleMatrix( angles, fRotateMatrix );

	Math::Vector vecPos;
	for ( int i = 0; i < 8; ++i )
	{
		vecPos.x = i & 0x1 ? vMaxs.x : vMins.x;
		vecPos.y = i & 0x2 ? vMaxs.y : vMins.y;
		vecPos.z = i & 0x4 ? vMaxs.z : vMins.z;

		VectorRotate( vecPos, fRotateMatrix, pVerts[i] );
		pVerts[i] += vOrigin;
	}
}

void Drawing::DrawBox( const Context& Context, const Math::Vector& Origin, const Math::Vector& Angle, const Math::Vector& Mins, const Math::Vector& Maxs, const Color& Color )
{
	Math::Vector WorldPoints[8];
	GenerateBoxVertices( Origin, Angle, Mins, Maxs, WorldPoints );

	struct ScreenPoint
	{
		bool OnScreen;
		Point Point;
	} ScreenPoints[8];

	for ( auto i = 0; i < 8; ++i )
		ScreenPoints[i].OnScreen = WorldToScreen( Context, WorldPoints[i], ScreenPoints[i].Point );

	glColor4ub( Color.r, Color.g, Color.b, Color.a );
	glBegin( GL_LINES );
	
	if ( ScreenPoints[0].OnScreen && ScreenPoints[1].OnScreen )
		DrawLineInternal( ScreenPoints[0].Point, ScreenPoints[1].Point );

	if ( ScreenPoints[0].OnScreen && ScreenPoints[2].OnScreen )
		DrawLineInternal( ScreenPoints[0].Point, ScreenPoints[2].Point );

	if ( ScreenPoints[0].OnScreen && ScreenPoints[4].OnScreen )
		DrawLineInternal( ScreenPoints[0].Point, ScreenPoints[4].Point );

	if ( ScreenPoints[1].OnScreen && ScreenPoints[3].OnScreen )
		DrawLineInternal( ScreenPoints[1].Point, ScreenPoints[3].Point );

	if ( ScreenPoints[1].OnScreen && ScreenPoints[5].OnScreen )
		DrawLineInternal( ScreenPoints[1].Point, ScreenPoints[5].Point );

	if ( ScreenPoints[2].OnScreen && ScreenPoints[3].OnScreen )
		DrawLineInternal( ScreenPoints[2].Point, ScreenPoints[3].Point );

	if ( ScreenPoints[2].OnScreen && ScreenPoints[6].OnScreen )
		DrawLineInternal( ScreenPoints[2].Point, ScreenPoints[6].Point );

	if ( ScreenPoints[3].OnScreen && ScreenPoints[7].OnScreen )
		DrawLineInternal( ScreenPoints[3].Point, ScreenPoints[7].Point );

	if ( ScreenPoints[4].OnScreen && ScreenPoints[5].OnScreen )
		DrawLineInternal( ScreenPoints[4].Point, ScreenPoints[5].Point );

	if ( ScreenPoints[4].OnScreen && ScreenPoints[6].OnScreen )
		DrawLineInternal( ScreenPoints[4].Point, ScreenPoints[6].Point );

	if ( ScreenPoints[5].OnScreen && ScreenPoints[7].OnScreen )
		DrawLineInternal( ScreenPoints[5].Point, ScreenPoints[7].Point );

	if ( ScreenPoints[6].OnScreen && ScreenPoints[7].OnScreen )
		DrawLineInternal( ScreenPoints[6].Point, ScreenPoints[7].Point );

	glEnd();
}

void Drawing::DrawSphere( const Context& Context, const Math::Vector& Center, double Radius, const Color& Color )
{
	constexpr auto SegmentsY = 24;
	constexpr auto SegmentsX = SegmentsY + 1;

	struct ScreenPoint
	{
		bool OnScreen;
		Point Point;
	} ScreenPoints[SegmentsX * SegmentsY];
	
	for ( auto y = 0; y < SegmentsY; ++y )
	{
		for ( auto x = 0; x < SegmentsX; ++x )
		{
			double sx, cx, sy, cy;
			Math::SinCos( Math::TAUd * ( x / static_cast<double>( SegmentsX - 1 ) ), sx, cx );
			Math::SinCos( Math::PId * ( y / static_cast<double>( SegmentsY - 1 ) ), sy, cy );

			auto& Point = ScreenPoints[y * SegmentsY + x];
			Point.OnScreen = WorldToScreen( Context, Math::Vector( Center.x + Radius * sy * cx, Center.y + Radius * cy, Center.z + Radius * sy * sx ), Point.Point );
		}
	}

	glColor4ub( Color.r, Color.g, Color.b, Color.a );
	glBegin( GL_LINES );
	
	for ( size_t i = 0; i < SegmentsY * ( SegmentsX - 2 ); ++i )
	{
		if ( !ScreenPoints[i].OnScreen )
			continue;

		if ( ScreenPoints[i + SegmentsX].OnScreen )
			DrawLineInternal( ScreenPoints[i].Point, ScreenPoints[i + SegmentsX].Point );

		if ( ScreenPoints[i + 1].OnScreen )
			DrawLineInternal( ScreenPoints[i].Point, ScreenPoints[i + 1].Point );
	}

	glEnd();
}