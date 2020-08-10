#pragma once

#include "Hook.h"

namespace Memory
{
	void InitMinHook();
	void UninitMinHook();
	
	class DetourHook : public Hook
	{
	public:
		template <typename T>
		DetourHook( const wchar_t* ModuleName, const char* FunctionName, T* NewFunction, void** Trampoline ) : DetourHook( ModuleName, FunctionName, reinterpret_cast<void*>( NewFunction ), Trampoline )
		{ }
		
		DetourHook( const wchar_t* ModuleName, const char* FunctionName, void* NewFunction, void** Trampoline );
		
		void Unhook() override;

	private:
		void* m_Target;
	};
}