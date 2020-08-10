#include "Config.h"
#include <Windows.h>
#include <fstream>
#include <string>
#include <charconv>

using namespace Minecraft;

Config::Config() :
	m_ForceSeed( 1 ),
	m_Seed( 2945671375784082726 ),
	m_ForceEnderPearls( 1 ),
	m_EnderPearls( 13 ),
	m_DrawCrystalHitboxes( 1 ),
	m_DrawBowTrajectory( 1 ),
	m_PanicKey( VK_NUMPAD0 )
{ }

void Config::Init()
{
	const auto Path = std::filesystem::current_path();
	const auto FilePath = Path / "msh_config.txt";
	if ( !exists( FilePath ) )
		MessageBox( nullptr, ( "Failed to find msh_config.txt.\nIt should be inside '" + Path.string() + "'.\nYou can create it now and after that close this window or the default settings will be used." ).c_str(),
					"Can't find config", MB_ICONWARNING );

	if ( !exists( FilePath ) )
		MessageBox( nullptr, "Default settings will be used.", "Can't find config", MB_ICONINFORMATION );
	else
		Load( FilePath );
}

bool Config::ShouldForceSeed() const
{
	return m_ForceSeed != 0;
}

uint64_t Config::GetSeed() const
{
	return m_Seed;
}

bool Config::ShouldForceEnderPearls() const
{
	return m_ForceEnderPearls != 0;
}

uint32_t Config::GetEnderPearlsAmount() const
{
	return m_EnderPearls;
}

bool Config::ShouldDrawCrystalHitboxes() const
{
	return m_DrawCrystalHitboxes != 0;
}

bool Config::ShouldDrawBowTrajectory() const
{
	return m_DrawBowTrajectory != 0;
}

uint32_t Config::GetPanicKey() const
{
	return m_PanicKey;
}

void Config::Load( const std::filesystem::path& Path )
{
	std::ifstream f( Path );
	if ( !f )
		return;

	std::string Line;
	while ( std::getline( f, Line ) )
	{
		auto End = Line.find_first_of( '#' );
		if ( End == std::string::npos )
			End = Line.length();

		ParseLine( std::string_view( Line.c_str(), End ) );
	}
}

void Config::ParseLine( const std::string_view& Line )
{
	const auto Delimiter = Line.find_first_of( '=' );
	if ( Delimiter == std::string_view::npos || Delimiter < 1 )
		return;

	static const auto Trim = []( std::string_view& String )
	{
		const auto Space = " \t\n\v\f\r";
		
		auto Begin = String.find_first_not_of( Space );
		if ( Begin == std::string_view::npos )
			return false;

		auto End = String.find_last_not_of( Space );
		if ( End == std::string_view::npos )
			return false;

		String = String.substr( Begin, End - Begin + 1 );
		
		return true;
	};
	
	auto Variable = Line.substr( 0, Delimiter );
	if ( !Trim( Variable ) )
		return;

	auto Value = Line.substr( Delimiter + 1 );
	Trim( Value );

	SetVariable( Variable, Value );
}

void Config::SetVariable( const std::string_view& Variable, const std::string_view& Value )
{
#define SET_VARIABLE( Name ) else if ( Variable == #Name ) std::from_chars( Value.data(), Value.data() + Value.length(), m_##Name )
	
	if ( Variable == "ForceSeed" )
		std::from_chars( Value.data(), Value.data() + Value.length(), m_ForceSeed );
	SET_VARIABLE( Seed );
	SET_VARIABLE( ForceEnderPearls );
	SET_VARIABLE( EnderPearls );
	SET_VARIABLE( DrawCrystalHitboxes );
	SET_VARIABLE( DrawBowTrajectory );
	SET_VARIABLE( PanicKey );
}