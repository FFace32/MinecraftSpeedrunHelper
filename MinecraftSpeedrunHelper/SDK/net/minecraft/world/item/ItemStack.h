#pragma once

#include <Java.h>

namespace net::minecraft::world::item
{
    class ItemLike;
    class Item;
	
    class ItemStack : public _jobject
    {
    public:
        DEFINE_THISCLASS( "bki" )
    	
        static auto init( ItemLike* item, jint count )
        {
            CALL_NEWOBJECT( ItemStack*, "(Lbqa;I)V", item, count );
        }

        auto getItem()
        {
            CALL_OBJECTMETHOD( Item*, "b", "()Lbke;" );
        }
    };
}