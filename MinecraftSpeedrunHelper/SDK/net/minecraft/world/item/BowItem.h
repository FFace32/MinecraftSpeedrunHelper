#pragma once

#include "Item.h"

namespace net::minecraft::world::item
{
	class BowItem : public Item
	{
	public:
		static float getPowerForTime( int ticks )
		{
			auto power = static_cast<float>( ticks ) / 20.f;
			if ( ( power = ( power * power + power * 2.f ) / 3.f ) > 1.f )
				power = 1.f;

			return power;
		}

		static jint getUseDuration()
		{
			return 72000;
		}
	};
}