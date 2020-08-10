#pragma once

#include <Java.h>

namespace java::util
{
    class OptionalLong : public _jobject
    {
    public:
        DEFINE_THISCLASS( "java/util/OptionalLong" )
    	
        static auto of( jlong value )
        {
            CALL_STATICOBJECTMETHOD( OptionalLong*, "of", "(J)Ljava/util/OptionalLong;", value );
        }
    };
}