#pragma once

#include <Java.h>

namespace java::lang
{
	class ClassLoader;

	class Thread : public _jobject
	{
	public:
		DEFINE_THISCLASS( "java/lang/Thread" )

		static auto currentThread()
		{
			CALL_STATICOBJECTMETHOD( Thread*, "currentThread", "()Ljava/lang/Thread;" );
		}

		auto getContextClassLoader()
		{
			CALL_OBJECTMETHOD( ClassLoader*, "getContextClassLoader", "()Ljava/lang/ClassLoader;" );
		}
	};
}