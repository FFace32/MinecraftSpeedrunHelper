#pragma once

#include "../../world/entity/LivingEntity.h"

namespace net::minecraft::client::player
{
    class Player : public world::entity::LivingEntity
    {
    public:
	    DEFINE_THISCLASS( "bft" )

    	jboolean canHarmPlayer( Player* player )
	    {
            CALL_BOOLEANMETHOD( "a", "(Lbft;)Z", player );
	    }
    };
}