#pragma once

#include <Java.h>

namespace net::minecraft::client
{
	namespace player
	{
		class LocalPlayer;
	}

	namespace renderer
	{
		class GameRenderer;
	}

	class Timer;
	
	class Minecraft : public _jobject
	{
	public:
		DEFINE_THISCLASS( "dlx" )

		auto timer()
		{
			GET_OBJECTFIELD( Timer*, "O", "Ldmj;" );
		}
		
		auto gameRenderer()
		{
			GET_OBJECTFIELD( renderer::GameRenderer*, "h", "Lebs;" );
		}

		auto player()
		{
			GET_OBJECTFIELD( player::LocalPlayer*, "s", "Lebf;" );
		}

		jboolean pause()
		{
			GET_BOOLEANFIELD( "aG" );
		}

		jfloat pausePartialTick()
		{
			GET_FLOATFIELD( "aH" );
		}
		
		static auto getInstance()
		{
			CALL_STATICOBJECTMETHOD( Minecraft*, "B", "()Ldlx;" );
		}
	};
}