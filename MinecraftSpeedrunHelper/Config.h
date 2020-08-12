#pragma once

#include <filesystem>
#include <string_view>
#include <cstdint>

namespace Minecraft
{
	class Config
	{
	public:
		Config();
		
		void Init();

		bool ShouldForceSeed() const;
		int64_t GetSeed() const;

		bool ShouldForceEnderPearls() const;
		uint32_t GetEnderPearlsAmount() const;

		bool ShouldDrawCrystalHitboxes() const;
		bool ShouldDrawBowTrajectory() const;

		uint32_t GetPanicKey() const;
		
	private:
		void Load( const std::filesystem::path& Path );
		void ParseLine( const std::string_view& Line );
		void SetVariable( const std::string_view& Variable, const std::string_view& Value );

		uint8_t m_ForceSeed;
		int64_t m_Seed;

		uint8_t m_ForceEnderPearls;
		uint32_t m_EnderPearls;

		uint8_t m_DrawCrystalHitboxes;
		uint8_t m_DrawBowTrajectory;

		uint32_t m_PanicKey;
	} inline g_Config;
}