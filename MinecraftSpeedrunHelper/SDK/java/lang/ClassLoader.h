#pragma once

#include <Java.h>

namespace java::lang
{
	class ClassLoader : public _jobject
	{
	public:
		DEFINE_THISCLASS( "java/lang/ClassLoader" )

		static auto getSystemClassLoader()
		{
			CALL_STATICOBJECTMETHOD( ClassLoader*, "getSystemClassLoader", "()Ljava/lang/ClassLoader;" );
		}

		auto loadClass( jstring name )
		{
			CALL_OBJECTMETHOD( jclass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;", name );
		}
	};
}