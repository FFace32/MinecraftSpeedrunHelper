#pragma once

#include <Java.h>

namespace java::lang
{
	class ClassLoader;
	
	class Class : public _jclass
	{
	public:
		DEFINE_THISCLASS( "java/lang/Class" )

		auto getClassLoader()
		{
			CALL_OBJECTMETHOD( ClassLoader*, "getClassLoader", "()Ljava/lang/ClassLoader;" );
		}
	};
}