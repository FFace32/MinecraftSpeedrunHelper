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
			DEFINE_THISCLASS( "aoy" )
			
			auto getItemInHand( InteractionHand* hand )
			{
				CALL_OBJECTMETHOD( item::ItemStack*, "b", "(Lanf;)Lbki;", hand );
			}

			jint getUseItemRemainingTicks()
			{
				CALL_INTMETHOD( "dY", "()I" );
			}
		};
	}
}