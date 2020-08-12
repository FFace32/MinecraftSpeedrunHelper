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
			DEFINE_THISCLASS( "apx" )
			
			auto level()
			{
				GET_OBJECTFIELD( level::Level*, "l", "Lbru;" );
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
				CALL_OBJECTMETHOD( EntityType*, "W", "()Laqb;" );
			}
			
			jboolean isOnGround()
			{
				CALL_BOOLEANMETHOD( "an", "()Z" );
			}

			jboolean isPickable()
			{
				CALL_BOOLEANMETHOD( "aS", "()Z" );
			}
			
			jboolean isAlive()
			{
				CALL_BOOLEANMETHOD( "aW", "()Z" );
			}

			auto getBoundingBox()
			{
				CALL_OBJECTMETHOD( phys::AABB*, "cb", "()Ldcf;" );
			}

			auto position()
			{
				CALL_OBJECTMETHOD( phys::Vec3*, "cz", "()Ldck;" );
			}

			jboolean isPassengerOfSameVehicle( Entity* entity )
			{
				CALL_BOOLEANMETHOD( "x", "(Lapx;)Z", entity );
			}
			
			auto getDeltaMovement()
			{
				CALL_OBJECTMETHOD( phys::Vec3*, "cB", "()Ldck;" );
			}

			jdouble getEyeY()
			{
				CALL_DOUBLEMETHOD( "cF", "()D" );
			}
		};
	}
}