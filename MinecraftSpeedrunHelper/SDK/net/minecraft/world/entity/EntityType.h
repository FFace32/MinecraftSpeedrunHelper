#pragma once

#include <Java.h>

namespace net::minecraft::world::entity
{
	class EntityType : public _jobject
	{
	public:
		DEFINE_THISCLASS( "aqb" )

		static auto AREA_EFFECT_CLOUD()
		{
			GET_STATICOBJECTFIELD( EntityType*, "a", "Laqb;" );
		}

		static auto ARMOR_STAND()
		{
			GET_STATICOBJECTFIELD( EntityType*, "b", "Laqb;" );
		}

		static auto ARROW()
		{
			GET_STATICOBJECTFIELD( EntityType*, "c", "Laqb;" );
		}

		static auto BAT()
		{
			GET_STATICOBJECTFIELD( EntityType*, "d", "Laqb;" );
		}

		static auto BEE()
		{
			GET_STATICOBJECTFIELD( EntityType*, "e", "Laqb;" );
		}

		static auto BLAZE()
		{
			GET_STATICOBJECTFIELD( EntityType*, "f", "Laqb;" );
		}

		static auto BOAT()
		{
			GET_STATICOBJECTFIELD( EntityType*, "g", "Laqb;" );
		}

		static auto CAT()
		{
			GET_STATICOBJECTFIELD( EntityType*, "h", "Laqb;" );
		}

		static auto CAVE_SPIDER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "i", "Laqb;" );
		}

		static auto CHICKEN()
		{
			GET_STATICOBJECTFIELD( EntityType*, "j", "Laqb;" );
		}

		static auto COD()
		{
			GET_STATICOBJECTFIELD( EntityType*, "k", "Laqb;" );
		}

		static auto COW()
		{
			GET_STATICOBJECTFIELD( EntityType*, "l", "Laqb;" );
		}

		static auto CREEPER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "m", "Laqb;" );
		}

		static auto DOLPHIN()
		{
			GET_STATICOBJECTFIELD( EntityType*, "n", "Laqb;" );
		}

		static auto DONKEY()
		{
			GET_STATICOBJECTFIELD( EntityType*, "o", "Laqb;" );
		}

		static auto DRAGON_FIREBALL()
		{
			GET_STATICOBJECTFIELD( EntityType*, "p", "Laqb;" );
		}

		static auto DROWNED()
		{
			GET_STATICOBJECTFIELD( EntityType*, "q", "Laqb;" );
		}

		static auto ELDER_GUARDIAN()
		{
			GET_STATICOBJECTFIELD( EntityType*, "r", "Laqb;" );
		}

		static auto END_CRYSTAL()
		{
			GET_STATICOBJECTFIELD( EntityType*, "s", "Laqb;" );
		}

		static auto ENDER_DRAGON()
		{
			GET_STATICOBJECTFIELD( EntityType*, "t", "Laqb;" );
		}

		static auto ENDERMAN()
		{
			GET_STATICOBJECTFIELD( EntityType*, "u", "Laqb;" );
		}

		static auto ENDERMITE()
		{
			GET_STATICOBJECTFIELD( EntityType*, "v", "Laqb;" );
		}

		static auto EVOKER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "w", "Laqb;" );
		}

		static auto EVOKER_FANGS()
		{
			GET_STATICOBJECTFIELD( EntityType*, "x", "Laqb;" );
		}

		static auto EXPERIENCE_ORB()
		{
			GET_STATICOBJECTFIELD( EntityType*, "y", "Laqb;" );
		}

		static auto EYE_OF_ENDER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "z", "Laqb;" );
		}

		static auto FALLING_BLOCK()
		{
			GET_STATICOBJECTFIELD( EntityType*, "A", "Laqb;" );
		}

		static auto FIREWORK_ROCKET()
		{
			GET_STATICOBJECTFIELD( EntityType*, "B", "Laqb;" );
		}

		static auto FOX()
		{
			GET_STATICOBJECTFIELD( EntityType*, "C", "Laqb;" );
		}

		static auto GHAST()
		{
			GET_STATICOBJECTFIELD( EntityType*, "D", "Laqb;" );
		}

		static auto GIANT()
		{
			GET_STATICOBJECTFIELD( EntityType*, "E", "Laqb;" );
		}

		static auto GUARDIAN()
		{
			GET_STATICOBJECTFIELD( EntityType*, "F", "Laqb;" );
		}

		static auto HOGLIN()
		{
			GET_STATICOBJECTFIELD( EntityType*, "G", "Laqb;" );
		}

		static auto HORSE()
		{
			GET_STATICOBJECTFIELD( EntityType*, "H", "Laqb;" );
		}

		static auto HUSK()
		{
			GET_STATICOBJECTFIELD( EntityType*, "I", "Laqb;" );
		}

		static auto ILLUSIONER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "J", "Laqb;" );
		}

		static auto IRON_GOLEM()
		{
			GET_STATICOBJECTFIELD( EntityType*, "K", "Laqb;" );
		}

		static auto ITEM()
		{
			GET_STATICOBJECTFIELD( EntityType*, "L", "Laqb;" );
		}

		static auto ITEM_FRAME()
		{
			GET_STATICOBJECTFIELD( EntityType*, "M", "Laqb;" );
		}

		static auto FIREBALL()
		{
			GET_STATICOBJECTFIELD( EntityType*, "N", "Laqb;" );
		}

		static auto LEASH_KNOT()
		{
			GET_STATICOBJECTFIELD( EntityType*, "O", "Laqb;" );
		}

		static auto LIGHTNING_BOLT()
		{
			GET_STATICOBJECTFIELD( EntityType*, "P", "Laqb;" );
		}

		static auto LLAMA()
		{
			GET_STATICOBJECTFIELD( EntityType*, "Q", "Laqb;" );
		}

		static auto LLAMA_SPIT()
		{
			GET_STATICOBJECTFIELD( EntityType*, "R", "Laqb;" );
		}

		static auto MAGMA_CUBE()
		{
			GET_STATICOBJECTFIELD( EntityType*, "S", "Laqb;" );
		}

		static auto MINECART()
		{
			GET_STATICOBJECTFIELD( EntityType*, "T", "Laqb;" );
		}

		static auto CHEST_MINECART()
		{
			GET_STATICOBJECTFIELD( EntityType*, "U", "Laqb;" );
		}

		static auto COMMAND_BLOCK_MINECART()
		{
			GET_STATICOBJECTFIELD( EntityType*, "V", "Laqb;" );
		}

		static auto FURNACE_MINECART()
		{
			GET_STATICOBJECTFIELD( EntityType*, "W", "Laqb;" );
		}

		static auto HOPPER_MINECART()
		{
			GET_STATICOBJECTFIELD( EntityType*, "X", "Laqb;" );
		}

		static auto SPAWNER_MINECART()
		{
			GET_STATICOBJECTFIELD( EntityType*, "Y", "Laqb;" );
		}

		static auto TNT_MINECART()
		{
			GET_STATICOBJECTFIELD( EntityType*, "Z", "Laqb;" );
		}

		static auto MULE()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aa", "Laqb;" );
		}

		static auto MOOSHROOM()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ab", "Laqb;" );
		}

		static auto OCELOT()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ac", "Laqb;" );
		}

		static auto PAINTING()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ad", "Laqb;" );
		}

		static auto PANDA()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ae", "Laqb;" );
		}

		static auto PARROT()
		{
			GET_STATICOBJECTFIELD( EntityType*, "af", "Laqb;" );
		}

		static auto PHANTOM()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ag", "Laqb;" );
		}

		static auto PIG()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ah", "Laqb;" );
		}

		static auto PIGLIN()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ai", "Laqb;" );
		}

		static auto PIGLIN_BRUTE()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aj", "Laqb;" );
		}
		
		static auto PILLAGER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ak", "Laqb;" );
		}

		static auto POLAR_BEAR()
		{
			GET_STATICOBJECTFIELD( EntityType*, "al", "Laqb;" );
		}

		static auto TNT()
		{
			GET_STATICOBJECTFIELD( EntityType*, "am", "Laqb;" );
		}

		static auto PUFFERFISH()
		{
			GET_STATICOBJECTFIELD( EntityType*, "an", "Laqb;" );
		}

		static auto RABBIT()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ao", "Laqb;" );
		}

		static auto RAVAGER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ap", "Laqb;" );
		}

		static auto SALMON()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aq", "Laqb;" );
		}

		static auto SHEEP()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ar", "Laqb;" );
		}

		static auto SHULKER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "as", "Laqb;" );
		}

		static auto SHULKER_BULLET()
		{
			GET_STATICOBJECTFIELD( EntityType*, "at", "Laqb;" );
		}

		static auto SILVERFISH()
		{
			GET_STATICOBJECTFIELD( EntityType*, "au", "Laqb;" );
		}

		static auto SKELETON()
		{
			GET_STATICOBJECTFIELD( EntityType*, "av", "Laqb;" );
		}

		static auto SKELETON_HORSE()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aw", "Laqb;" );
		}

		static auto SLIME()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ax", "Laqb;" );
		}

		static auto SMALL_FIREBALL()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ay", "Laqb;" );
		}

		static auto SNOW_GOLEM()
		{
			GET_STATICOBJECTFIELD( EntityType*, "az", "Laqb;" );
		}

		static auto SNOWBALL()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aA", "Laqb;" );
		}

		static auto SPECTRAL_ARROW()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aB", "Laqb;" );
		}

		static auto SPIDER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aC", "Laqb;" );
		}

		static auto SQUID()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aD", "Laqb;" );
		}

		static auto STRAY()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aE", "Laqb;" );
		}

		static auto STRIDER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aF", "Laqb;" );
		}

		static auto EGG()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aG", "Laqb;" );
		}

		static auto ENDER_PEARL()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aH", "Laqb;" );
		}

		static auto EXPERIENCE_BOTTLE()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aI", "Laqb;" );
		}

		static auto POTION()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aJ", "Laqb;" );
		}

		static auto TRIDENT()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aK", "Laqb;" );
		}

		static auto TRADER_LLAMA()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aL", "Laqb;" );
		}

		static auto TROPICAL_FISH()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aM", "Laqb;" );
		}

		static auto TURTLE()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aN", "Laqb;" );
		}

		static auto VEX()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aO", "Laqb;" );
		}

		static auto VILLAGER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aP", "Laqb;" );
		}

		static auto VINDICATOR()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aQ", "Laqb;" );
		}

		static auto WANDERING_TRADER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aR", "Laqb;" );
		}

		static auto WITCH()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aS", "Laqb;" );
		}

		static auto WITHER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aT", "Laqb;" );
		}

		static auto WITHER_SKELETON()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aU", "Laqb;" );
		}

		static auto WITHER_SKULL()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aV", "Laqb;" );
		}

		static auto WOLF()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aW", "Laqb;" );
		}

		static auto ZOGLIN()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aX", "Laqb;" );
		}

		static auto ZOMBIE()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aY", "Laqb;" );
		}

		static auto ZOMBIE_HORSE()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aZ", "Laqb;" );
		}

		static auto ZOMBIE_VILLAGER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ba", "Laqb;" );
		}

		static auto ZOMBIFIED_PIGLIN()
		{
			GET_STATICOBJECTFIELD( EntityType*, "bb", "Laqb;" );
		}

		static auto PLAYER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "bc", "Laqb;" );
		}

		static auto FISHING_BOBBER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "bd", "Laqb;" );
		}
	};
}