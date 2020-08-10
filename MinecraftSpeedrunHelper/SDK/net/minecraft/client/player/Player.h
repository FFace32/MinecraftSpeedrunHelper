#pragma once

#include "../../world/entity/LivingEntity.h"

namespace net::minecraft::client::player
{
    class Player : public world::entity::LivingEntity
    {
    public:
	    DEFINE_THISCLASS( "bec" )

    	jboolean canHarmPlayer( Player* player )
	    {
            CALL_BOOLEANMETHOD( "a", "(Lbec;)Z", player );
	    }
    };
}