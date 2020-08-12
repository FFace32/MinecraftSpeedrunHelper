#pragma once

#include <Java.h>

namespace net::minecraft::world::phys
{
	class Vec3;

	class HitResult : public _jobject
	{
	public:
		DEFINE_THISCLASS( "dci" )

		class Type : public _jobject
		{
		public:
			DEFINE_THISCLASS( "dci$a" )

			static auto MISS()
			{
				GET_STATICOBJECTFIELD( Type*, "a", "Ldci$a;" );
			}

			static auto ENTITY()
			{
				GET_STATICOBJECTFIELD( Type*, "c", "Ldci$a;" );
			}
		};

		auto getType()
		{
			CALL_OBJECTMETHOD( Type*, "c", "()Ldci$a;" );
		}

		auto getLocation()
		{
			CALL_OBJECTMETHOD( Vec3*, "e", "()Ldck;" );
		}
	};
}