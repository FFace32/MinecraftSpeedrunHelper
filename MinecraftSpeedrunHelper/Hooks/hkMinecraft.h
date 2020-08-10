#pragma once

#include "../cppjavahook/JavaHook.h"

namespace Minecraft::Hooks
{
	void makeSettings( JNIEnv* Env, const JavaHook::HookData& HookData );

	void getBarterResponseItems( JNIEnv* Env, const JavaHook::HookData& HookData );
}