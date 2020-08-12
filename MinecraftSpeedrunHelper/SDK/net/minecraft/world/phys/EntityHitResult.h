#pragma once

#include "HitResult.h"

namespace net::minecraft::world::phys
{
	class EntityHitResult : public HitResult
	{
	public:
		DEFINE_THISCLASS( "dch" )

		auto getEntity()
		{
			CALL_OBJECTMETHOD( entity::Entity*, "a", "()Lapx;" );
		}
	};
}