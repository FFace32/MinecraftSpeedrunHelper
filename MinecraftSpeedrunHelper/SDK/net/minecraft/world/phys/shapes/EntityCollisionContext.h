#pragma once

#include "CollisionContext.h"

namespace net::minecraft::world::phys
{
	class EntityCollisionContext : public CollisionContext
	{
	public:
		DEFINE_THISCLASS( "dcu" )

		void descending( jboolean value )
		{
			SET_BOOLEANFIELD( "b", value );
		}
		
		void entityBottom( jdouble Value )
		{
			SET_DOUBLEFIELD( "c", Value );
		}
	};
}