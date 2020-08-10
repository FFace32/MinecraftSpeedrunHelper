#pragma once

#include "ItemLike.h"

namespace net::minecraft::world::item
{
    class Item : public ItemLike
    {
    public:
        DEFINE_THISCLASS( "bke" )
    	
        static jint getId( Item* item )
        {
            CALL_STATICINTMETHOD( "a", "(Lbke;)I", item );
        }

        static auto byId( jint id )
        {
            CALL_STATICOBJECTMETHOD( Item*, "b", "(I)Lbke;", id );
        }
    };
}