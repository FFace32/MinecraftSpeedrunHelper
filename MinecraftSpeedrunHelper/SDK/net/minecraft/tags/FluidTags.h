#pragma once

#include "Tag.h"

namespace net::minecraft::tags
{
	class Tag;
	
	class FluidTags : public _jobject
	{
	public:
		DEFINE_THISCLASS( "aed" )

		static auto WATER()
		{
			GET_STATICOBJECTFIELD( Tag*, "b", "Laej$e;" );
		}
	};
}