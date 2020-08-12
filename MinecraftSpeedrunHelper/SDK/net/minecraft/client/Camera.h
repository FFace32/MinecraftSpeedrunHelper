#pragma once

#include <Java.h>

namespace com::mojang::math
{
	class Vector3f;
}

namespace net::minecraft
{
	namespace world::phys
	{
		class Vec3;
	}
	
	namespace client
	{
		class Camera : public _jobject
		{
		public:
			DEFINE_THISCLASS( "djh" )
			
			auto position()
			{
				GET_OBJECTFIELD( world::phys::Vec3*, "d", "Ldck;" );
			}

			auto forwards()
			{
				GET_OBJECTFIELD( com::mojang::math::Vector3f*, "f", "Lg;" );
			}

			auto up()
			{
				GET_OBJECTFIELD( com::mojang::math::Vector3f*, "g", "Lg;" );
			}

			auto left()
			{
				GET_OBJECTFIELD( com::mojang::math::Vector3f*, "h", "Lg;" );
			}
		};
	}
}