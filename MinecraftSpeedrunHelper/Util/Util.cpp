#include "Util.h"
#include <random>

int Random( int Min, int Max )
{
	static std::random_device Device;
	static std::mt19937 Generator( Device() );
	
	return std::uniform_int_distribution<>( Min, Max )( Generator );
}