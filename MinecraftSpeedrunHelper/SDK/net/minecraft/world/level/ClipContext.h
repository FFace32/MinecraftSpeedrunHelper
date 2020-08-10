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
			DEFINE_THISCLASS( "bpj" )

			class Block : public _jobject
			{
			public:
				DEFINE_THISCLASS( "bpj$a" )

				static auto COLLIDER()
				{
					GET_STATICOBJECTFIELD( Block*, "a", "Lbpj$a;" );
				}
			};

			class Fluid : public _jobject
			{
			public:
				DEFINE_THISCLASS( "bpj$b" )

				static auto NONE()
				{
					GET_STATICOBJECTFIELD( Fluid*, "a", "Lbpj$b;" );
				}
			};

			auto collisionContext()
			{
				GET_OBJECTFIELD( phys::CollisionContext*, "e", "Lder;" );
			}
			
			static auto init( phys::Vec3* from, phys::Vec3* to, Block* block, Fluid* fluid, entity::Entity* entity )
			{
				CALL_NEWOBJECT( ClipContext*, "(Ldem;Ldem;Lbpj$a;Lbpj$b;Laom;)V", from, to, block, fluid, entity );
			}
		};
	}
}