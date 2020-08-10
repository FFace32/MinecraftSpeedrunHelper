#pragma once

#include "CollisionContext.h"

namespace net::minecraft::world::phys
{
	class EntityCollisionContext : public CollisionContext
	{
	public:
		DEFINE_THISCLASS( "dew" )

		void descending( jboolean value )
		{
			SET_BOOLEANFIELD( "b", value );
		}
		
		void entityBottom( jdouble Value )
		{
			SET_DOUBLEFIELD( "c", Value );
		}

		void heldItem( item::Item* Value )
		{
			SET_OBJECTFIELD( "d", "Lbke;", Value );
		}
	};
}