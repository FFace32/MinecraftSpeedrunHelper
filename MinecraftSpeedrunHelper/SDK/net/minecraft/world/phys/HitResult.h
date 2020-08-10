#pragma once

#include <Java.h>

namespace net::minecraft::world::phys
{
	class Vec3;

	class HitResult : public _jobject
	{
	public:
		DEFINE_THISCLASS( "dej" )

		class Type : public _jobject
		{
		public:
			DEFINE_THISCLASS( "dej$a" )

			static auto MISS()
			{
				GET_STATICOBJECTFIELD( Type*, "a", "Ldej$a;" );
			}

			static auto ENTITY()
			{
				GET_STATICOBJECTFIELD( Type*, "c", "Ldej$a;" );
			}
		};

		auto getType()
		{
			CALL_OBJECTMETHOD( Type*, "c", "()Ldej$a;" );
		}

		auto getLocation()
		{
			CALL_OBJECTMETHOD( Vec3*, "e", "()Ldem;" );
		}
	};
}