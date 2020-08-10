#pragma once

#include <Java.h>

namespace net::minecraft::core
{
	class BlockPos : public _jobject
	{
	public:
		DEFINE_THISCLASS( "fu" )

		class MutableBlockPos;
		
		static auto init( jint x, jint y, jint z )
		{
			CALL_NEWOBJECT( BlockPos*, "(III)V", x, y, z );
		}
	};

	class BlockPos::MutableBlockPos : public BlockPos
	{
	public:
		DEFINE_THISCLASS( "fu$a" )

		static auto init()
		{
			CALL_NEWOBJECT( MutableBlockPos*, "()V" );
		}
		
		auto set( jint x, jint y, jint z )
		{
			CALL_OBJECTMETHOD( MutableBlockPos*, "d", "(III)Lfu$a;", x, y, z );
		}
	};
}