#pragma once

#include <thread>

template <long long Duration, typename Fn>
auto SleepFunction( const Fn& Func )
{
	auto Return = Func();
	while ( !Return )
	{
		Return = Func();

		std::this_thread::sleep_for( std::chrono::milliseconds( Duration ) );
	}

	return Return;
}

int Random( int Min, int Max );