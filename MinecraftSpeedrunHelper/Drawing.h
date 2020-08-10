#pragma once

#include "Math/AABB.h"

namespace net::minecraft::client
{
	class Minecraft;
}

namespace Drawing
{
	class Point
	{
	public:
		Point() = default;
		Point( int x, int y );
		
		int x, y;
	};

	class Color
	{
	public:
		Color( uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255 );
		
		uint8_t r, g, b, a;
	};
	
	struct Context
	{
		Context( int Width, int Height, net::minecraft::client::Minecraft* Minecraft, float LineWidth );
		
		Point ScreenCenter;

		struct
		{
			Math::Vector Position;
			Math::Vector Forwards;
			Math::Vector Up;
			Math::Vector Left;
			double FovX;
			double FovY;
		} Camera;

		float LineWidth;
	};
	
	Context Begin( net::minecraft::client::Minecraft* Minecraft );
	void End( const Context& Context );

	bool WorldToScreen( const Context& Context, const Math::Vector& World, Point& Screen );

	void DrawLine( const Color& Color, const Point& Location0, const Point& Location1 );
	void DrawLine( const Color& Color, int x0, int y0, int x1, int y1 );

	void DrawBox( const Context& Context, const Math::AABB& AABB, const Color& Color );
	void DrawBox( const Context& Context, const Math::Vector& Origin, const Math::Vector& Angle, const Math::Vector& Mins, const Math::Vector& Maxs, const Color& Color );

	void DrawSphere( const Context& Context, const Math::Vector& Center, double Radius, const Color& Color );
}