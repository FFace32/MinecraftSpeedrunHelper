#pragma once

#include "Entity.h"

namespace net::minecraft::world
{
	class InteractionHand;

	namespace item
	{
		class ItemStack;
	}
	
	namespace entity
	{
		class LivingEntity : public Entity
		{
		public:
			DEFINE_THISCLASS( "aqj" )
			
			auto getItemInHand( InteractionHand* hand )
			{
				CALL_OBJECTMETHOD( item::ItemStack*, "b", "(Laoq;)Lbly;", hand );
			}

			jint getUseItemRemainingTicks()
			{
				CALL_INTMETHOD( "dY", "()I" );
			}
		};
	}
}