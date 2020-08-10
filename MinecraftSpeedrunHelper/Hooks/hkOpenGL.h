#pragma once

#include <Windows.h>

namespace Minecraft::Hooks
{
	inline void* owglSwapBuffers;
	BOOL WINAPI wglSwapBuffers( HDC DC );
}