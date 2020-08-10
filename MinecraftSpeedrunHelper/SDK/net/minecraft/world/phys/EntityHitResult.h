#pragma once

#include "HitResult.h"

namespace net::minecraft::world::phys
{
	class EntityHitResult : public HitResult
	{
	public:
		DEFINE_THISCLASS( "dei" )

		auto getEntity()
		{
			CALL_OBJECTMETHOD( entity::Entity*, "a", "()Laom;" );
		}
	};
}