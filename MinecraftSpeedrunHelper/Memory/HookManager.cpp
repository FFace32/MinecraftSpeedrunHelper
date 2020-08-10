#include "HookManager.h"
#include "DetourHook.h"
#include <thread>

using namespace Memory;

HookManager::HookManager()
{
	InitMinHook();
}

Hook* HookManager::RegisterHook( Hook* Hook )
{
	return m_Hooks.emplace_back( Hook );
}

void HookManager::Unhook()
{
	if ( m_Hooks.empty() )
		return UninitMinHook();

	for ( const auto& Hook : m_Hooks )
		Hook->Unhook();

	std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) ); // Hopefully 500ms are enough for all the hooked functions to leave the stack

	for ( const auto& Hook : m_Hooks )
		delete Hook;
	m_Hooks.clear();

	UninitMinHook();
}