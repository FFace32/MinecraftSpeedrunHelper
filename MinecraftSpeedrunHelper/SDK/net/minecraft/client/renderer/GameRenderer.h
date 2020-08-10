#pragma once

#include <Java.h>

namespace net::minecraft::client
{
	class Camera;
	
	namespace renderer
	{
		class GameRenderer : public _jobject
		{
		public:
			DEFINE_THISCLASS( "ebs" )
			
			jdouble getFov( Camera* camera, jfloat tick, jboolean useSettingsFov )
			{
				CALL_DOUBLEMETHOD( "b", "(Ldlj;FZ)D", camera, tick, useSettingsFov );
			}

			auto getMainCamera()
			{
				CALL_OBJECTMETHOD( Camera*, "k", "()Ldlj;" );
			}
		};
	}
}