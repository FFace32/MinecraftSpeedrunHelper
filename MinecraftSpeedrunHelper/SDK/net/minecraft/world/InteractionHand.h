#pragma once

#include <Java.h>

namespace net::minecraft::world
{
    class InteractionHand : public _jobject
    {
    public:
        DEFINE_THISCLASS( "anf" )
    	
        static auto MAIN_HAND()
        {
            GET_STATICOBJECTFIELD( InteractionHand*, "a", "Lanf;" );
        }

        static auto OFF_HAND()
        {
            GET_STATICOBJECTFIELD( InteractionHand*, "b", "Lanf;" );
        }
    };
}