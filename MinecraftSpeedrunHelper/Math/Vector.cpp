#include "Vector.h"

using namespace Math;

Vector::Vector( const JObjectGuard<net::minecraft::world::phys::Vec3*>& Vec3 ) :
	x( Vec3->x() ),
	y( Vec3->y() ),
	z( Vec3->z() )
{ }

Vector::Vector( const JObjectGuard<com::mojang::math::Vector3f*>& Vector3f ) :
	x( static_cast<double>( Vector3f->x() ) ),
	y( static_cast<double>( Vector3f->y() ) ),
	z( static_cast<double>( Vector3f->z() ) )
{ }

constexpr Vector::Vector( double X, double Y, double Z ) :
	x( X ),
	y( Y ),
	z( Z )
{ }

Vector Vector::operator+( const Vector& Rhs ) const
{
	return Vector( x + Rhs.x, y + Rhs.y, z + Rhs.z );
}

Vector Vector::operator-( const Vector& Rhs ) const
{
	return Vector( x - Rhs.x, y - Rhs.y, z - Rhs.z );
}

Vector& Vector::operator+=( const Vector& Rhs )
{
	x += Rhs.x;
	y += Rhs.y;
	z += Rhs.z;
	return *this;
}

Vector& Vector::operator+=( double Rhs )
{
	x += Rhs;
	y += Rhs;
	z += Rhs;
	return *this;
}

Vector& Vector::operator-=( double Rhs )
{
	x -= Rhs;
	y -= Rhs;
	z -= Rhs;
	return *this;
}

Vector& Vector::operator*=( double Rhs )
{
	x *= Rhs;
	y *= Rhs;
	z *= Rhs;
	return *this;
}

double Vector::Dot( const Vector& Rhs ) const
{
	return x * Rhs.x + y * Rhs.y + z * Rhs.z;
}

double Vector::Dot( const double* Rhs ) const
{
	return x * Rhs[0] + y * Rhs[1] + z * Rhs[2];
}

Vector& Vector::Normalize()
{
	const auto Length = sqrt( x * x + y * y + z * z );
	if ( Length )
		*this *= 1.0 / Length;

	return *this;
}

double Vector::GetHorizontalDistance() const
{
	return sqrt( x * x + z * z );
}

JObjectGuard<net::minecraft::world::phys::Vec3*> Vector::ToMinecraft() const
{
	return net::minecraft::world::phys::Vec3::init( x, y, z );
}