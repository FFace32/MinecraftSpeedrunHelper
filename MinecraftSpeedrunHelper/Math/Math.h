#pragma once

#include <cmath>

namespace Math
{
	template <typename T>
	constexpr T PI = static_cast<T>( 3.1415926535897932384626433 );

	template <typename T>
	constexpr T TAU = PI<T> * 2;

	template <typename T>
	constexpr T PHI = static_cast<T>( 1.6180339887498948482045868 );

	constexpr float PIf = PI<float>;
	constexpr double PId = PI<double>;
	constexpr long double PIl = PI<long double>;

	constexpr float TAUf = TAU<float>;
	constexpr double TAUd = TAU<double>;
	constexpr long double TAUl = TAU<long double>;

	constexpr float PHIf = PHI<float>;
	constexpr double PHId = PHI<double>;
	constexpr long double PHIl = PHI<long double>;

	template <typename T>
	constexpr T RadToDeg( const T& Rad )
	{
		return Rad * T( 180 ) / PI<T>;
	}

	template <typename T>
	constexpr T DegToRad( const T& Deg )
	{
		return Deg * PI<T> / T( 180 );
	}

	template <typename T>
	T cosd( T d )
	{
		return cos( DegToRad( d ) );
	}

	template <typename T>
	T sind( T d )
	{
		return sin( DegToRad( d ) );
	}
	
	template <typename T>
	T tand( T d )
	{
		return tan( DegToRad( d ) );
	}

	template <typename T>
	void SinCos( T r, T& s, T& c )
	{
		s = sin( r );
		c = cos( r );
	}

	template <typename T>
	void SinCosD( T r, T& s, T& c )
	{
		SinCos( DegToRad( r ), s, c );
	}

	float AngleNormalize( float Angle );

	using Matrix3x4 = double[3][4];
}