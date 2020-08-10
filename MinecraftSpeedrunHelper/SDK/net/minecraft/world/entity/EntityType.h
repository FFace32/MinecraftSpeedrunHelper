#pragma once

#include <Java.h>

namespace net::minecraft::world::entity
{
	class EntityType : public _jobject
	{
	public:
		DEFINE_THISCLASS( "aoq" )

		static auto AREA_EFFECT_CLOUD()
		{
			GET_STATICOBJECTFIELD( EntityType*, "a", "Laoq;" );
		}

		static auto ARMOR_STAND()
		{
			GET_STATICOBJECTFIELD( EntityType*, "b", "Laoq;" );
		}

		static auto ARROW()
		{
			GET_STATICOBJECTFIELD( EntityType*, "c", "Laoq;" );
		}

		static auto BAT()
		{
			GET_STATICOBJECTFIELD( EntityType*, "d", "Laoq;" );
		}

		static auto BEE()
		{
			GET_STATICOBJECTFIELD( EntityType*, "e", "Laoq;" );
		}

		static auto BLAZE()
		{
			GET_STATICOBJECTFIELD( EntityType*, "f", "Laoq;" );
		}

		static auto BOAT()
		{
			GET_STATICOBJECTFIELD( EntityType*, "g", "Laoq;" );
		}

		static auto CAT()
		{
			GET_STATICOBJECTFIELD( EntityType*, "h", "Laoq;" );
		}

		static auto CAVE_SPIDER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "i", "Laoq;" );
		}

		static auto CHICKEN()
		{
			GET_STATICOBJECTFIELD( EntityType*, "j", "Laoq;" );
		}

		static auto COD()
		{
			GET_STATICOBJECTFIELD( EntityType*, "k", "Laoq;" );
		}

		static auto COW()
		{
			GET_STATICOBJECTFIELD( EntityType*, "l", "Laoq;" );
		}

		static auto CREEPER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "m", "Laoq;" );
		}

		static auto DOLPHIN()
		{
			GET_STATICOBJECTFIELD( EntityType*, "n", "Laoq;" );
		}

		static auto DONKEY()
		{
			GET_STATICOBJECTFIELD( EntityType*, "o", "Laoq;" );
		}

		static auto DRAGON_FIREBALL()
		{
			GET_STATICOBJECTFIELD( EntityType*, "p", "Laoq;" );
		}

		static auto DROWNED()
		{
			GET_STATICOBJECTFIELD( EntityType*, "q", "Laoq;" );
		}

		static auto ELDER_GUARDIAN()
		{
			GET_STATICOBJECTFIELD( EntityType*, "r", "Laoq;" );
		}

		static auto END_CRYSTAL()
		{
			GET_STATICOBJECTFIELD( EntityType*, "s", "Laoq;" );
		}

		static auto ENDER_DRAGON()
		{
			GET_STATICOBJECTFIELD( EntityType*, "t", "Laoq;" );
		}

		static auto ENDERMAN()
		{
			GET_STATICOBJECTFIELD( EntityType*, "u", "Laoq;" );
		}

		static auto ENDERMITE()
		{
			GET_STATICOBJECTFIELD( EntityType*, "v", "Laoq;" );
		}

		static auto EVOKER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "w", "Laoq;" );
		}

		static auto EVOKER_FANGS()
		{
			GET_STATICOBJECTFIELD( EntityType*, "x", "Laoq;" );
		}

		static auto EXPERIENCE_ORB()
		{
			GET_STATICOBJECTFIELD( EntityType*, "y", "Laoq;" );
		}

		static auto EYE_OF_ENDER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "z", "Laoq;" );
		}

		static auto FALLING_BLOCK()
		{
			GET_STATICOBJECTFIELD( EntityType*, "A", "Laoq;" );
		}

		static auto FIREWORK_ROCKET()
		{
			GET_STATICOBJECTFIELD( EntityType*, "B", "Laoq;" );
		}

		static auto FOX()
		{
			GET_STATICOBJECTFIELD( EntityType*, "C", "Laoq;" );
		}

		static auto GHAST()
		{
			GET_STATICOBJECTFIELD( EntityType*, "D", "Laoq;" );
		}

		static auto GIANT()
		{
			GET_STATICOBJECTFIELD( EntityType*, "E", "Laoq;" );
		}

		static auto GUARDIAN()
		{
			GET_STATICOBJECTFIELD( EntityType*, "F", "Laoq;" );
		}

		static auto HOGLIN()
		{
			GET_STATICOBJECTFIELD( EntityType*, "G", "Laoq;" );
		}

		static auto HORSE()
		{
			GET_STATICOBJECTFIELD( EntityType*, "H", "Laoq;" );
		}

		static auto HUSK()
		{
			GET_STATICOBJECTFIELD( EntityType*, "I", "Laoq;" );
		}

		static auto ILLUSIONER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "J", "Laoq;" );
		}

		static auto IRON_GOLEM()
		{
			GET_STATICOBJECTFIELD( EntityType*, "K", "Laoq;" );
		}

		static auto ITEM()
		{
			GET_STATICOBJECTFIELD( EntityType*, "L", "Laoq;" );
		}

		static auto ITEM_FRAME()
		{
			GET_STATICOBJECTFIELD( EntityType*, "M", "Laoq;" );
		}

		static auto FIREBALL()
		{
			GET_STATICOBJECTFIELD( EntityType*, "N", "Laoq;" );
		}

		static auto LEASH_KNOT()
		{
			GET_STATICOBJECTFIELD( EntityType*, "O", "Laoq;" );
		}

		static auto LIGHTNING_BOLT()
		{
			GET_STATICOBJECTFIELD( EntityType*, "P", "Laoq;" );
		}

		static auto LLAMA()
		{
			GET_STATICOBJECTFIELD( EntityType*, "Q", "Laoq;" );
		}

		static auto LLAMA_SPIT()
		{
			GET_STATICOBJECTFIELD( EntityType*, "R", "Laoq;" );
		}

		static auto MAGMA_CUBE()
		{
			GET_STATICOBJECTFIELD( EntityType*, "S", "Laoq;" );
		}

		static auto MINECART()
		{
			GET_STATICOBJECTFIELD( EntityType*, "T", "Laoq;" );
		}

		static auto CHEST_MINECART()
		{
			GET_STATICOBJECTFIELD( EntityType*, "U", "Laoq;" );
		}

		static auto COMMAND_BLOCK_MINECART()
		{
			GET_STATICOBJECTFIELD( EntityType*, "V", "Laoq;" );
		}

		static auto FURNACE_MINECART()
		{
			GET_STATICOBJECTFIELD( EntityType*, "W", "Laoq;" );
		}

		static auto HOPPER_MINECART()
		{
			GET_STATICOBJECTFIELD( EntityType*, "X", "Laoq;" );
		}

		static auto SPAWNER_MINECART()
		{
			GET_STATICOBJECTFIELD( EntityType*, "Y", "Laoq;" );
		}

		static auto TNT_MINECART()
		{
			GET_STATICOBJECTFIELD( EntityType*, "Z", "Laoq;" );
		}

		static auto MULE()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aa", "Laoq;" );
		}

		static auto MOOSHROOM()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ab", "Laoq;" );
		}

		static auto OCELOT()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ac", "Laoq;" );
		}

		static auto PAINTING()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ad", "Laoq;" );
		}

		static auto PANDA()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ae", "Laoq;" );
		}

		static auto PARROT()
		{
			GET_STATICOBJECTFIELD( EntityType*, "af", "Laoq;" );
		}

		static auto PHANTOM()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ag", "Laoq;" );
		}

		static auto PIG()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ah", "Laoq;" );
		}

		static auto PIGLIN()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ai", "Laoq;" );
		}

		static auto PILLAGER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aj", "Laoq;" );
		}

		static auto POLAR_BEAR()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ak", "Laoq;" );
		}

		static auto TNT()
		{
			GET_STATICOBJECTFIELD( EntityType*, "al", "Laoq;" );
		}

		static auto PUFFERFISH()
		{
			GET_STATICOBJECTFIELD( EntityType*, "am", "Laoq;" );
		}

		static auto RABBIT()
		{
			GET_STATICOBJECTFIELD( EntityType*, "an", "Laoq;" );
		}

		static auto RAVAGER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ao", "Laoq;" );
		}

		static auto SALMON()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ap", "Laoq;" );
		}

		static auto SHEEP()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aq", "Laoq;" );
		}

		static auto SHULKER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ar", "Laoq;" );
		}

		static auto SHULKER_BULLET()
		{
			GET_STATICOBJECTFIELD( EntityType*, "as", "Laoq;" );
		}

		static auto SILVERFISH()
		{
			GET_STATICOBJECTFIELD( EntityType*, "at", "Laoq;" );
		}

		static auto SKELETON()
		{
			GET_STATICOBJECTFIELD( EntityType*, "au", "Laoq;" );
		}

		static auto SKELETON_HORSE()
		{
			GET_STATICOBJECTFIELD( EntityType*, "av", "Laoq;" );
		}

		static auto SLIME()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aw", "Laoq;" );
		}

		static auto SMALL_FIREBALL()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ax", "Laoq;" );
		}

		static auto SNOW_GOLEM()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ay", "Laoq;" );
		}

		static auto SNOWBALL()
		{
			GET_STATICOBJECTFIELD( EntityType*, "az", "Laoq;" );
		}

		static auto SPECTRAL_ARROW()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aA", "Laoq;" );
		}

		static auto SPIDER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aB", "Laoq;" );
		}

		static auto SQUID()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aC", "Laoq;" );
		}

		static auto STRAY()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aD", "Laoq;" );
		}

		static auto STRIDER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aE", "Laoq;" );
		}

		static auto EGG()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aF", "Laoq;" );
		}

		static auto ENDER_PEARL()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aG", "Laoq;" );
		}

		static auto EXPERIENCE_BOTTLE()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aH", "Laoq;" );
		}

		static auto POTION()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aI", "Laoq;" );
		}

		static auto TRIDENT()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aJ", "Laoq;" );
		}

		static auto TRADER_LLAMA()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aK", "Laoq;" );
		}

		static auto TROPICAL_FISH()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aL", "Laoq;" );
		}

		static auto TURTLE()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aM", "Laoq;" );
		}

		static auto VEX()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aN", "Laoq;" );
		}

		static auto VILLAGER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aO", "Laoq;" );
		}

		static auto VINDICATOR()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aP", "Laoq;" );
		}

		static auto WANDERING_TRADER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aQ", "Laoq;" );
		}

		static auto WITCH()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aR", "Laoq;" );
		}

		static auto WITHER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aS", "Laoq;" );
		}

		static auto WITHER_SKELETON()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aT", "Laoq;" );
		}

		static auto WITHER_SKULL()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aU", "Laoq;" );
		}

		static auto WOLF()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aV", "Laoq;" );
		}

		static auto ZOGLIN()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aW", "Laoq;" );
		}

		static auto ZOMBIE()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aX", "Laoq;" );
		}

		static auto ZOMBIE_HORSE()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aY", "Laoq;" );
		}

		static auto ZOMBIE_VILLAGER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "aZ", "Laoq;" );
		}

		static auto ZOMBIFIED_PIGLIN()
		{
			GET_STATICOBJECTFIELD( EntityType*, "ba", "Laoq;" );
		}

		static auto PLAYER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "bb", "Laoq;" );
		}

		static auto FISHING_BOBBER()
		{
			GET_STATICOBJECTFIELD( EntityType*, "bc", "Laoq;" );
		}
	};
}