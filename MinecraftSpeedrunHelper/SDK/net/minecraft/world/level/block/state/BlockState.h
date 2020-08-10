#pragma once

#include "BlockBehaviour.h"

namespace net::minecraft::world::level::block::state
{
	class BlockState : public BlockBehaviour::BlockStateBase
	{
	public:
		DEFINE_THISCLASS( "cfj" )
	};
}