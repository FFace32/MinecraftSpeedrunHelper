#pragma once

#include <vector>

namespace Memory
{
	class Hook;

	class HookManager
	{
	public:
		HookManager();
		
		template <typename T>
		T* RegisterHook( T* THook )
		{
			return reinterpret_cast<T*>( RegisterHook( reinterpret_cast<Hook*>( THook ) ) );
		}
		
		Hook* RegisterHook( Hook* Hook );

		void Unhook();
		
	private:
		std::vector<Hook*> m_Hooks;
	} inline g_HookManager;
}