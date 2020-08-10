#include "AABB.h"
#include "../SDK/net/minecraft/world/phys/AABB.h"

using namespace Math;

AABB::AABB( const Vector& MinsMaxs ) :
	Mins( MinsMaxs ),
	Maxs( MinsMaxs )
{ }

AABB::AABB( const JObjectGuard<net::minecraft::world::phys::AABB*>& AABB ) :
	Mins( AABB->minX(), AABB->minY(), AABB->minZ() ),
	Maxs( AABB->maxX(), AABB->maxY(), AABB->maxZ() )
{ }

AABB::AABB( double MinX, double MinY, double MinZ, double MaxX, double MaxY, double MaxZ ) :
	Mins( MinX, MinY, MinZ ),
	Maxs( MaxX, MaxY, MaxZ )
{ }

AABB& AABB::ExpandTowards( const Vector& Vector )
{
	if ( Vector.x < 0.0 )
		Mins.x += Vector.x;
	else
		Maxs.x += Vector.x;

	if ( Vector.y < 0.0 )
		Mins.y += Vector.y;
	else
		Maxs.y += Vector.y;

	if ( Vector.z < 0.0 )
		Mins.z += Vector.z;
	else
		Maxs.z += Vector.z;

	return *this;
}

AABB& AABB::Inflate( double Amount )
{
	Mins -= Amount;
	Maxs += Amount;
	return *this;
}

AABB& AABB::Deflate( double Amount )
{
	Mins += Amount;
	Maxs -= Amount;
	return *this;
}

JObjectGuard<net::minecraft::world::phys::AABB*> AABB::ToMinecraft() const
{
	return net::minecraft::world::phys::AABB::init( Mins.x, Mins.y, Mins.z, Maxs.x, Maxs.y, Maxs.z );
}