#pragma once

#include <Java.h>

namespace net::minecraft
{
	namespace core
	{
		class BlockPos;
	}

	namespace tags
	{
		class Tag;
	}

	namespace world::level
	{
		class BlockGetter;

		namespace material
		{
			class FluidState : public _jobject
			{
			public:
				DEFINE_THISCLASS( "cuu" )

				jfloat getHeight( BlockGetter* blockGetter, core::BlockPos* blockPos )
				{
					CALL_FLOATMETHOD( "a", "(Lbqz;Lfx;)F", blockGetter, blockPos );
				}

				jboolean is( tags::Tag* tag )
				{
					CALL_BOOLEANMETHOD( "a", "(Laej;)Z", tag );
				}
			};
		}
	}
}