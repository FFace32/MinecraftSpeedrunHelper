#pragma once

#include "../AABB.h"
#include "../../../../../java/util/ArrayList.h"

namespace net::minecraft::world::phys
{
	namespace shapes
	{
		class VoxelShape : public _jobject
		{
		public:
			DEFINE_THISCLASS( "dde" )
			
			jboolean isEmpty()
			{
				CALL_BOOLEANMETHOD( "b", "()Z" );
			}

			auto toAabbs()
			{
				CALL_OBJECTMETHOD( java::util::ArrayList<AABB>*, "d", "()Ljava/util/List;" );
			}
		};
	}
}