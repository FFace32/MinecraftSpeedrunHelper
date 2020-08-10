#pragma once

#include <Java.h>

namespace net::minecraft::world
{
	namespace phys
	{
		class BlockHitResult;
	}
	
	namespace level
	{
		class ClipContext;

		class BlockGetter : public _jobject
		{
		public:
			DEFINE_THISCLASS( "bpg" )
			
			auto clip( ClipContext* context )
			{
				CALL_OBJECTMETHOD( phys::BlockHitResult*, "a", "(Lbpj;)Ldeh;", context );
			}
		};
	}
}