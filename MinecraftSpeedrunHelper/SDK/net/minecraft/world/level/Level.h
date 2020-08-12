#pragma once

#include "BlockGetter.h"
#include "SDK/java/util/ArrayList.h"

namespace net::minecraft
{
	namespace core
	{
		class BlockPos;
	}

	namespace world
	{
		namespace phys
		{
			class AABB;
		}

		namespace entity
		{
			class Entity;
		}
		
		namespace level
		{
			namespace material
			{
				class FluidState;
			}

			namespace block::state
			{
				class BlockState;
			}

			class Level : public BlockGetter // TO DO: Figure out how to implement multiple inheritance without breaking the memory layout
			{
			public:
				DEFINE_THISCLASS( "bru" )

				auto getBlockState( core::BlockPos* block )
				{
					CALL_OBJECTMETHOD( block::state::BlockState*, "d_", "(Lfx;)Lcee;", block );
				}

				auto getFluidState( core::BlockPos* block )
				{
					CALL_OBJECTMETHOD( material::FluidState*, "b", "(Lfx;)Lcuu;", block );
				}

				auto getEntities( entity::Entity* ignore, phys::AABB* AABB, jobject predicate )
				{
					CALL_OBJECTMETHOD( java::util::ArrayList<entity::Entity>*, "a", "(Lapx;Ldcf;Ljava/util/function/Predicate;)Ljava/util/List;", ignore, AABB, predicate );
				}
			};
		}
	}
}