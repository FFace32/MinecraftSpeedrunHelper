#pragma once

namespace Memory
{
	class Hook
	{
	public:
		virtual ~Hook() = default;
		
		virtual void Unhook() = 0;
	};
}