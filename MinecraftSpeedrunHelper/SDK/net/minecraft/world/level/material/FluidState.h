#pragma once

#include <Java.h>

namespace net::minecraft
{
	namespace tags
	{
		class Tag;
	}
	
	namespace world::level::material
	{
		class FluidState : public _jobject
		{
		public:
			DEFINE_THISCLASS( "cxa" )

			jfloat getHeight( BlockGetter* blockGetter, core::BlockPos* blockPos )
			{
				CALL_FLOATMETHOD( "a", "(Lbpg;Lfu;)F", blockGetter, blockPos );
			}
			
			jboolean is( tags::Tag* tag )
			{
				CALL_BOOLEANMETHOD( "a", "(Ladf;)Z", tag );
			}
		};
	}
}