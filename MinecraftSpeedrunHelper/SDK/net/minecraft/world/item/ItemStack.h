#pragma once

#include <Java.h>

namespace net::minecraft::world
{
    namespace level
    {
        class ItemLike;
    }

    namespace item
    {
        class Item;

        class ItemStack : public _jobject
        {
        public:
            DEFINE_THISCLASS( "bly" )

            static auto init( level::ItemLike* item, jint count )
            {
                CALL_NEWOBJECT( ItemStack*, "(Lbrt;I)V", item, count );
            }

            auto getItem()
            {
                CALL_OBJECTMETHOD( Item*, "b", "()Lblu;" );
            }
        };
    }
}