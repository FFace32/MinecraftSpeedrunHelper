#include <Windows.h>
#include "Java.h"
#include "Config.h"
#include "cppjavahook/JavaHook.h"
#include "Memory/DetourHook.h"
#include "Memory/HookManager.h"
#include "Hooks/hkMinecraft.h"
#include "Hooks/hkOpenGL.h"
#include "Util/Util.h"

using namespace Minecraft;

DWORD WINAPI Init( LPVOID hinstDLL )
{
	try
	{
		g_Java.Init();
		g_Config.Init();

		using Memory::g_HookManager;

		auto MinecraftHook = g_HookManager.RegisterHook( new JavaHook() );
		MinecraftHook->AddHook( "cix"/*net.minecraft.world.level.levelgen.WorldGenSettings*/, "a"/*withSeed*/,
								"(ZLjava/util/OptionalLong;)Lcix;"/*(boolean,java.util.OptionalLong)net.minecraft.world.level.levelgen.WorldGenSettings*/, Hooks::makeSettings );

		MinecraftHook->AddHook( "bdd"/*net.minecraft.world.entity.monster.piglin.PiglinAi*/, "m"/*getBarterResponseItems*/,
								"(Lbdc;)Ljava/util/List;"/*(net.minecraft.world.entity.monster.piglin.Piglin)java.util.List*/, Hooks::getBarterResponseItems );

		MinecraftHook->Init( g_Java.GetEnv(), g_Java.GetJVMTIEnv() );
		
		g_HookManager.RegisterHook( new Memory::DetourHook( L"opengl32.dll", "wglSwapBuffers", Hooks::wglSwapBuffers, &Hooks::owglSwapBuffers ) );

		SleepFunction<500>( []()
		{
			return GetAsyncKeyState( g_Config.GetPanicKey() );
		} );

		g_HookManager.Unhook();
		g_Java.Release();

		FreeLibraryAndExitThread( reinterpret_cast<HMODULE>( hinstDLL ), EXIT_SUCCESS );
	}
	catch ( const std::exception& Exception )
	{
		MessageBox( nullptr, Exception.what(), "Exception", MB_ICONERROR );

		throw;
	}
}

BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID )
{
	if ( fdwReason == DLL_PROCESS_ATTACH )
		CreateThread( nullptr, 0, Init, hinstDLL, 0, nullptr );

	return TRUE;
}