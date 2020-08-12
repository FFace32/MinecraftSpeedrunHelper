#pragma once

#include <Java.h>

namespace net::minecraft::client
{
	class Timer : public _jobject
	{
	public:
		DEFINE_THISCLASS( "dkh" )

		jfloat partialTick()
		{
			GET_FLOATFIELD( "a" );
		}
	};
}