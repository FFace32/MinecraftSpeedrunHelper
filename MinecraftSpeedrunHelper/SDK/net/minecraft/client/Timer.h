#pragma once

#include <Java.h>

namespace net::minecraft::client
{
	class Timer : public _jobject
	{
	public:
		DEFINE_THISCLASS( "dmj" )

		jfloat partialTick()
		{
			GET_FLOATFIELD( "a" );
		}
	};
}