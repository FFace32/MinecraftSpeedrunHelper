#pragma once

#include <Java.h>

namespace net::minecraft::world
{
	namespace entity
	{
		class Entity;
	}

	namespace phys
	{
		class CollisionContext;
		class Vec3;
	}
	
	namespace level
	{
		class ClipContext : public _jobject
		{
		public:
			DEFINE_THISCLASS( "brc" )

			class Block : public _jobject
			{
			public:
				DEFINE_THISCLASS( "brc$a" )

				static auto COLLIDER()
				{
					GET_STATICOBJECTFIELD( Block*, "a", "Lbrc$a;" );
				}
			};

			class Fluid : public _jobject
			{
			public:
				DEFINE_THISCLASS( "brc$b" )

				static auto NONE()
				{
					GET_STATICOBJECTFIELD( Fluid*, "a", "Lbrc$b;" );
				}
			};

			auto collisionContext()
			{
				GET_OBJECTFIELD( phys::CollisionContext*, "e", "Ldcp;" );
			}
			
			static auto init( phys::Vec3* from, phys::Vec3* to, Block* block, Fluid* fluid, entity::Entity* entity )
			{
				CALL_NEWOBJECT( ClipContext*, "(Ldck;Ldck;Lbrc$a;Lbrc$b;Lapx;)V", from, to, block, fluid, entity );
			}
		};
	}
}