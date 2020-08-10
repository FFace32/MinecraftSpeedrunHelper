#include "hkMinecraft.h"
#include "../SDK/java/util/OptionalLong.h"
#include "../SDK/java/util/ArrayList.h"
#include "../SDK/net/minecraft/world/item/Item.h"
#include "../SDK/net/minecraft/world/item/ItemStack.h"
#include "../SDK/net/minecraft/world/item/Items.h"
#include "../SDK/net/minecraft/client/Minecraft.h"
#include "../SDK/net/minecraft/client/player/LocalPlayer.h"
#include "../SDK/net/minecraft/world/level/Level.h"
#include "../Config.h"
#include "../Util/Util.h"

using namespace Minecraft;

void Hooks::makeSettings( JNIEnv* Env, const JavaHook::HookData& HookData ) // cix/*net.minecraft.world.level.levelgen.WorldGenSettings*/ (0 = cix this, 1 = boolean hardcore, 2 = java.util.OptionalLong seed)
{
	g_Java.SetEnv( Env ); // Doing this in every hook is recommended

	if ( g_Config.ShouldForceSeed() )
		HookData.SetArgument( 2, java::util::OptionalLong::of( g_Config.GetSeed() ) );
}

void Hooks::getBarterResponseItems( JNIEnv* Env, const JavaHook::HookData& HookData ) // java.util.List (0 = dbc/*net.minecraft.world.entity.monster.piglin.Piglin*/ piglin)
{
	g_Java.SetEnv( Env ); // Doing this in every hook is recommended

	if ( g_Config.ShouldForceEnderPearls() )
	{
		static uint32_t DroppedEnderPearls = 0;
		static JObjectGuard<net::minecraft::world::level::Level*> LastLevel;

		auto Level = net::minecraft::client::Minecraft::getInstance()->player()->level();
		if ( !g_Java.GetEnv()->IsSameObject( LastLevel, Level ) ) // Reset the counter if we reload/load another world
		{
			LastLevel = std::move( Level );
			DroppedEnderPearls = 0;
		}

		if ( DroppedEnderPearls < g_Config.GetEnderPearlsAmount() )
		{
			auto MinAmount = 4;
			auto MaxAmount = g_Config.GetEnderPearlsAmount() % 8;

			const auto RequiredAmount = g_Config.GetEnderPearlsAmount() - DroppedEnderPearls;
			if ( RequiredAmount / static_cast<float>( 8 ) >= 2.f )
				MinAmount = MaxAmount = 8;
			else
			{
				if ( MaxAmount < 4 )
					MaxAmount += 4;
				else
				{
					MinAmount = MaxAmount;
					MaxAmount = 8;
				}
			}
			
			if ( RequiredAmount <= 8 )
				MinAmount = MaxAmount = RequiredAmount;

			const auto DropAmount = Random( MinAmount, MaxAmount );
			DroppedEnderPearls += DropAmount;

			using namespace net::minecraft::world::item;
			const auto ItemStacks = java::util::ArrayList<ItemStack>::init();
			ItemStacks->add( ItemStack::init( Item::byId( Items::ENDER_PEARL ), DropAmount ) );
			
			HookData.SetReturn( ItemStacks );
		}
	}
}