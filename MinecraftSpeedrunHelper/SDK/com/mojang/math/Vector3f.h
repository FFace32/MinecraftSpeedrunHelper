#pragma once

#include <Java.h>

namespace com::mojang::math
{
	class Vector3f : public _jobject
	{
	public:
		DEFINE_THISCLASS( "g" )

		jfloat x()
		{
			CALL_FLOATMETHOD( "a", "()F" );
		}

		jfloat y()
		{
			CALL_FLOATMETHOD( "b", "()F" );
		}

		jfloat z()
		{
			CALL_FLOATMETHOD( "c", "()F" );
		}
	};
}