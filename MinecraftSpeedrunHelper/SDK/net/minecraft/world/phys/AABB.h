#pragma once

#include <Java.h>

namespace net::minecraft
{
	namespace core
	{
		class BlockPos;
	}

	namespace world::phys
	{
		class Vec3;
		
		class AABB : public _jobject
		{
		public:
			DEFINE_THISCLASS( "deg" )

			jdouble minX()
			{
				GET_DOUBLEFIELD( "a" );
			}

			jdouble minY()
			{
				GET_DOUBLEFIELD( "b" );
			}

			jdouble minZ()
			{
				GET_DOUBLEFIELD( "c" );
			}

			jdouble maxX()
			{
				GET_DOUBLEFIELD( "d" );
			}

			jdouble maxY()
			{
				GET_DOUBLEFIELD( "e" );
			}

			jdouble maxZ()
			{
				GET_DOUBLEFIELD( "f" );
			}
			
			static auto init( jdouble minX, jdouble minY, jdouble minZ, jdouble maxX, jdouble maxY, jdouble maxZ )
			{
				CALL_NEWOBJECT( AABB*, "(DDDDDD)V", minX, minY, minZ, maxX, maxY, maxZ );
			}
			
			auto move( core::BlockPos* blockPos )
			{
				CALL_OBJECTMETHOD( AABB*, "a", "(Lfu;)Ldeg;", blockPos );
			}

			jboolean contains( Vec3* pos )
			{
				CALL_BOOLEANMETHOD( "d", "(Ldem;)Z", pos );
			}
		};
	}
}