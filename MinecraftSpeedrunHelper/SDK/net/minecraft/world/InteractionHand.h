#pragma once

#include <Java.h>

namespace net::minecraft::world
{
    class InteractionHand : public _jobject
    {
    public:
        DEFINE_THISCLASS( "aoq" )
    	
        static auto MAIN_HAND()
        {
            GET_STATICOBJECTFIELD( InteractionHand*, "a", "Laoq;" );
        }

        static auto OFF_HAND()
        {
            GET_STATICOBJECTFIELD( InteractionHand*, "b", "Laoq;" );
        }
    };
}