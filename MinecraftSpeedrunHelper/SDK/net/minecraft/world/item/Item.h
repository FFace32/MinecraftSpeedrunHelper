#pragma once

#include "../level/ItemLike.h"

namespace net::minecraft::world::item
{
    class Item : public level::ItemLike
    {
    public:
        DEFINE_THISCLASS( "blu" )
    	
        static jint getId( Item* item )
        {
            CALL_STATICINTMETHOD( "a", "(Lblu;)I", item );
        }

        static auto byId( jint id )
        {
            CALL_STATICOBJECTMETHOD( Item*, "b", "(I)Lblu;", id );
        }
    };
}