#pragma once

#include "Vector.h"

namespace net::minecraft::world::phys
{
	class AABB;
}

namespace Math
{
	class AABB
	{
	public:
		AABB( const Vector& MinsMaxs );
		AABB( const JObjectGuard<net::minecraft::world::phys::AABB*>& AABB );
		AABB( double MinX, double MinY, double MinZ, double MaxX, double MaxY, double MaxZ );

		AABB& ExpandTowards( const Vector& Vector );
		AABB& Inflate( double Amount );
		AABB& Deflate( double Amount );
		
		JObjectGuard<net::minecraft::world::phys::AABB*> ToMinecraft() const;
		
		Vector Mins;
		Vector Maxs;
	};
}