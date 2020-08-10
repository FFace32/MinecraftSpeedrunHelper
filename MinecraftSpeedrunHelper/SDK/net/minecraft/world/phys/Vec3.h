#pragma once

#include <Java.h>

namespace net::minecraft::world::phys
{
	class Vec3 : public _jobject
	{
	public:
		DEFINE_THISCLASS( "dem" )
		
		static auto init( jdouble x, jdouble y, jdouble z )
		{
			CALL_NEWOBJECT( Vec3*, "(DDD)V", x, y, z );
		}
		
		jdouble x()
		{
			CALL_DOUBLEMETHOD( "a", "()D" ); // TO DO: Use fields more
		}

		jdouble y()
		{
			CALL_DOUBLEMETHOD( "b", "()D" );
		}

		jdouble z()
		{
			CALL_DOUBLEMETHOD( "c", "()D" );
		}
	};
}