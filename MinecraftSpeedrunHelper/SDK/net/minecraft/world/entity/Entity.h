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
		class Vec3;
	}
	
	namespace entity
	{
		class EntityType;
		
		class Entity : public _jobject
		{
		public:
			DEFINE_THISCLASS( "aom" )
			
			auto level()
			{
				GET_OBJECTFIELD( level::Level*, "l", "Lbqb;" );
			}

			jfloat yRot()
			{
				GET_FLOATFIELD( "p" );
			}

			jfloat xRot()
			{
				GET_FLOATFIELD( "q" );
			}

			jboolean isSpectator()
			{
				CALL_BOOLEANMETHOD( "a_", "()Z" );
			}

			auto getType()
			{
				CALL_OBJECTMETHOD( EntityType*, "U", "()Laoq;" );
			}
			
			jboolean isOnGround()
			{
				CALL_BOOLEANMETHOD( "aj", "()Z" );
			}

			jboolean isPickable()
			{
				CALL_BOOLEANMETHOD( "aQ", "()Z" );
			}
			
			jboolean isAlive()
			{
				CALL_BOOLEANMETHOD( "aU", "()Z" );
			}

			auto getBoundingBox()
			{
				CALL_OBJECTMETHOD( phys::AABB*, "cb", "()Ldeg;" );
			}
			
			jfloat getBbWidth()
			{
				CALL_FLOATMETHOD( "cx", "()F" );
			}

			jfloat getBbHeight()
			{
				CALL_FLOATMETHOD( "cy", "()F" );
			}
			
			auto position()
			{
				CALL_OBJECTMETHOD( phys::Vec3*, "cz", "()Ldem;" );
			}

			jboolean isPassengerOfSameVehicle( Entity* entity )
			{
				CALL_BOOLEANMETHOD( "x", "(Laom;)Z", entity );
			}
			
			auto getDeltaMovement()
			{
				CALL_OBJECTMETHOD( phys::Vec3*, "cB", "()Ldem;" );
			}

			jdouble getEyeY()
			{
				CALL_DOUBLEMETHOD( "cF", "()D" );
			}
		};
	}
}