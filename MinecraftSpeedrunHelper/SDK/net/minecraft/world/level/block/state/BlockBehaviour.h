#pragma once

#include <Java.h>

namespace net::minecraft
{
	namespace core
	{
		class BlockPos;
	}
	
	namespace world
	{
		namespace phys::shapes
		{
			class VoxelShape;
		}

		namespace level
		{
			class BlockGetter;
			
			namespace block::state
			{
				class BlockBehaviour
				{
				public:
					class BlockStateBase : public _jobject
					{
					public:
						DEFINE_THISCLASS( "ced$a" )

						jboolean isAir()
						{
							CALL_BOOLEANMETHOD( "g", "()Z" );
						}

						auto getCollisionShape( BlockGetter* getter, core::BlockPos* block )
						{
							CALL_OBJECTMETHOD( phys::shapes::VoxelShape*, "k", "(Lbqz;Lfx;)Ldde;", getter, block );
						}
					};
				};
			}
		}
	}
}