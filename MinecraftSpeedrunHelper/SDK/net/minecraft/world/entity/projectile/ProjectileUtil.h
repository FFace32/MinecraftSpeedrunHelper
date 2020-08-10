#pragma once

#include <Java.h>

namespace net::minecraft::world
{
	namespace level
	{
		class Level;
	}

	namespace phys
	{
		class AABB;
		class EntityHitResult;
		class Vec3;
	}
	
	namespace entity
	{
		class Entity;

		namespace projectile
		{
			class ProjectileUtil : public _jobject
			{
			public:
				DEFINE_THISCLASS( "bet" )
				
				static auto getEntityHitResult( level::Level* level, Entity* ignore, phys::Vec3* from, phys::Vec3* to, phys::AABB* aabb, jobject predicate/*Creating a predicate from JNI is pretty hard so no thanks, just pass null*/ )
				{
					CALL_STATICOBJECTMETHOD( phys::EntityHitResult*, "a", "(Lbqb;Laom;Ldem;Ldem;Ldeg;Ljava/util/function/Predicate;)Ldei;", level, ignore, from, to, aabb, predicate );
				}
			};
		}
	}
}