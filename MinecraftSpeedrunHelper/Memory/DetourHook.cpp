#include "DetourHook.h"
#include "../minhook/include/MinHook.h"
#include <stdexcept>
#include <string>
#include "../Util/Util.h"

using namespace Memory;

void Memory::InitMinHook()
{
	if ( auto Error = MH_Initialize(); Error )
		throw std::runtime_error( std::string( "Failed to initialize minhook: " ) + MH_StatusToString( Error ) );
}

void Memory::UninitMinHook()
{
	MH_Uninitialize();
}

DetourHook::DetourHook( const wchar_t* ModuleName, const char* FunctionName, void* NewFunction, void** Trampoline )
{
	MH_STATUS Error;
	SleepFunction<500>( [&]() // In case the module is lazy loaded.
	{
		if ( ( Error = MH_CreateHookApiEx( ModuleName, FunctionName, NewFunction, Trampoline, &m_Target ) ) == MH_ERROR_MODULE_NOT_FOUND )
			return false;

		return true;
	} );
	
	if ( Error )
		throw std::runtime_error( std::string( "Failed to hook function: " ) + MH_StatusToString( Error ) );

	if ( ( Error = MH_EnableHook( m_Target ) ) )
		throw std::runtime_error( std::string( "Failed to enable hook: " ) + MH_StatusToString( Error ) );
}

void DetourHook::Unhook()
{
	MH_RemoveHook( m_Target );
}