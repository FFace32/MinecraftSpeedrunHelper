#pragma once

#include "../SDK/net/minecraft/world/phys/Vec3.h"
#include "../SDK/com/mojang/math/Vector3f.h"

namespace Math
{
	class Vector
	{
	public:
		Vector() = default;
		Vector( const JObjectGuard<net::minecraft::world::phys::Vec3*>& Vec3 );
		Vector( const JObjectGuard<com::mojang::math::Vector3f*>& Vector3f );
		constexpr Vector( double X, double Y, double Z );

		Vector operator+( const Vector& Rhs ) const;
		Vector operator-( const Vector& Rhs ) const;

		Vector& operator+=( const Vector& Rhs );
		Vector& operator+=( double Rhs );
		
		Vector& operator-=( double Rhs );

		Vector& operator*=( double Rhs );
		
		double Dot( const Vector& Rhs ) const;

		// Just for VectorRotate
		double Dot( const double* Rhs ) const;

		Vector& Normalize();

		double GetHorizontalDistance() const;

		JObjectGuard<net::minecraft::world::phys::Vec3*> ToMinecraft() const;
		
		static constexpr Vector ZERO()
		{
			return Vector( 0.0, 0.0, 0.0 );
		}
		
		double x, y, z;
	};
}