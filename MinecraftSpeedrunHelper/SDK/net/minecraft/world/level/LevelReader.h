#pragma once

#include <Java.h>

namespace net::minecraft::world::level
{
	class LevelReader : public _jobject
	{
	public:
		DEFINE_THISCLASS( "brw" )

		jboolean hasChunksAt( jint minX, jint minY, jint minZ, jint maxX, jint maxY, jint maxZ )
		{
			CALL_BOOLEANMETHOD( "a", "(IIIIII)Z", minX, minY, minZ, maxX, maxY, maxZ );
		}
	};
}