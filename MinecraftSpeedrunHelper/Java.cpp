#include "Java.h"
#include <algorithm>
#include <stdexcept>
#include <string>
#include "SDK/java/lang/ClassLoader.h"

Java::~Java()
{
	Release();
}

void Java::Init()
{
	if ( !m_VM )
	{
		jsize Count;
		if ( auto Error = JNI_GetCreatedJavaVMs( &m_VM, 1, &Count ); Error != JNI_OK || Count == 0 )
			throw std::runtime_error( "GetCreatedJavaVMs failed, " + std::to_string( Error ) );
	}
	
	// Inside hooked Java functions, the class loader belongs to cppjavahook and I won't be able to load Minecraft's classes
	m_ClassLoader = java::lang::ClassLoader::getSystemClassLoader();
}

JNIEnv* Java::GetEnv() const
{
	if ( !s_Env )
	{
		auto Error = m_VM->GetEnv( reinterpret_cast<void**>( &s_Env ), JNI_VERSION_1_1 );
		if ( Error == JNI_EDETACHED )
			Error = m_VM->AttachCurrentThread( reinterpret_cast<void**>( &s_Env ), nullptr ); // Hopefully this gets called only from Init

		if ( Error )
			throw std::runtime_error( "Failed to get JNIEnv, " + std::to_string( Error ) );
	}

	return s_Env;
}

jvmtiEnv* Java::GetJVMTIEnv()
{
	if ( !m_JVMTIEnv )
	{
		if ( auto Error = m_VM->GetEnv( reinterpret_cast<void**>( &m_JVMTIEnv ), JVMTI_VERSION_1_1 ); Error )
			throw std::runtime_error( "Failed to get JVMTIEnv, " + std::to_string( Error ) );
	}

	return m_JVMTIEnv;
}

void Java::SetEnv( JNIEnv* Env )
{
	s_Env = Env;

	if ( !m_VM )
	{
		if ( auto Error = s_Env->GetJavaVM( &m_VM ); Error )
			throw std::runtime_error( "GetJavaVM failed, " + std::to_string( Error ) );
	}
}

jclass Java::FindClass( const char* Name )
{
	jclass Class;

	if ( !m_ClassLoader )
		Class = GetEnv()->FindClass( Name );
	else
	{
		// Convert to Java class name format
		std::string JavaName = Name;
		std::replace( JavaName.begin(), JavaName.end(), '/', '.' );
		
		const auto String = GetEnv()->NewStringUTF( JavaName.c_str() );
		Class = m_ClassLoader->loadClass( String ).Release();
		GetEnv()->DeleteLocalRef( String );
	}

	if ( !Class )
		return nullptr; // Throw maybe

	const auto GlobalClass = reinterpret_cast<jclass>( GetEnv()->NewGlobalRef( Class ) );
	if ( !GlobalClass )
		return nullptr;

	GetEnv()->DeleteLocalRef( Class );

	m_Classes.emplace_back( GlobalClass );
	
	return GlobalClass;
}

void Java::Release()
{
	if ( !m_VM )
		return;

	for ( const auto& Class : m_Classes )
		GetEnv()->DeleteGlobalRef( Class );
	m_Classes.clear();

	m_VM->DetachCurrentThread();
	m_VM = nullptr;
}