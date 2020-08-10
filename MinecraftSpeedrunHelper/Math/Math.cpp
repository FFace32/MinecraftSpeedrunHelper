#include "Math.h"

using namespace Math;

float Math::AngleNormalize( float Angle )
{
	if( !std::isfinite( Angle ) )
		return 0.f;

	if( Angle > 180.f || Angle < -180.f )
	{
		const auto Rotations = round( abs( Angle / 360.f ) ) * 360.f;

		if( Angle < 0.f )
			return Angle + Rotations;

		return Angle - Rotations;
	}

	return Angle;
}