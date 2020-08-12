#pragma once

#include <jni.h>
#include <jvmti.h>
#include <vector>

namespace java::lang
{
	class ClassLoader;
}

// This could be better but I wrote it after I already did most of the SDK and I won't change the SDK now...
template <typename T>
class JObjectGuard
{
	static_assert( std::is_pointer_v<T>, "JObjectGuard is meant to be used with pointers only" );

public:
	JObjectGuard() :
		m_Object( nullptr )
	{ }

	explicit JObjectGuard( T Object ) :
		m_Object( Object )
	{ }

	JObjectGuard( JObjectGuard&& ObjectGuard ) noexcept
	{
		swap( *this, ObjectGuard );
	}

	explicit JObjectGuard( std::nullptr_t ) :
		m_Object( nullptr )
	{ }

	JObjectGuard( const JObjectGuard& ) = delete;

	~JObjectGuard();

	JObjectGuard& operator=( JObjectGuard&& Rhs ) noexcept
	{
		swap( *this, Rhs );
		return *this;
	}

	JObjectGuard& operator=( std::nullptr_t )
	{
		Reset();

		return *this;
	}

	JObjectGuard& operator=( const JObjectGuard& ) = delete;

	T operator->() const
	{
		return m_Object;
	}

	operator T() const
	{
		return m_Object;
	}

	template <typename U>
	JObjectGuard<U> CastTo()
	{
		const auto Copy = reinterpret_cast<U>( m_Object );
		m_Object = nullptr;

		return JObjectGuard<U>( Copy );
	}

	T Release()
	{
		const auto Return = m_Object;
		m_Object = nullptr;
		return Return;
	}

	void Reset();

	friend void swap( JObjectGuard& A, JObjectGuard& B ) noexcept
	{
		std::swap( A.m_Object, B.m_Object );
	}

private:
	T m_Object;
};

class Java
{
public:
	~Java();

	void Init();
	
	JNIEnv* GetEnv() const;
	jvmtiEnv* GetJVMTIEnv();
	void SetEnv( JNIEnv* Env );

	jclass FindClass( const char* Name );
	void Release();

private:
	JavaVM* m_VM = nullptr;
	jvmtiEnv* m_JVMTIEnv = nullptr;
	std::vector<jclass> m_Classes;
	JObjectGuard<java::lang::ClassLoader*> m_ClassLoader;
	
	static inline thread_local JNIEnv* s_Env = nullptr;
} inline g_Java;

template <typename T>
JObjectGuard<T>::~JObjectGuard()
{
	if ( m_Object )
		g_Java.GetEnv()->DeleteLocalRef( reinterpret_cast<jobject>( m_Object ) );
}

template <typename T>
void JObjectGuard<T>::Reset()
{
	if ( auto Object = Release() )
		g_Java.GetEnv()->DeleteLocalRef( reinterpret_cast<jobject>( Object ) );
}

#define DEFINE_THISCLASS( Name )	static jclass GetThisClass() \
									{ \
										static auto Class = g_Java.FindClass( Name ); \
										return Class; \
									}

#define CALL_NEWOBJECT( ReturnType, Descriptor, ... )	static auto init = g_Java.GetEnv()->GetMethodID( GetThisClass(), "<init>", Descriptor ); \
														return JObjectGuard( reinterpret_cast<ReturnType>( g_Java.GetEnv()->NewObject( GetThisClass(), init, __VA_ARGS__ ) ) )

#define CALL_OBJECTMETHOD( ReturnType, Name, Descriptor, ... )	static auto Method = g_Java.GetEnv()->GetMethodID( GetThisClass(), Name, Descriptor ); \
																return JObjectGuard( reinterpret_cast<ReturnType>( g_Java.GetEnv()->CallObjectMethod( this, Method, __VA_ARGS__ ) ) )

#define CALL_BOOLEANMETHOD( Name, Descriptor, ... )	static auto Method = g_Java.GetEnv()->GetMethodID( GetThisClass(), Name, Descriptor ); \
													return g_Java.GetEnv()->CallBooleanMethod( this, Method, __VA_ARGS__ )

#define CALL_INTMETHOD( Name, Descriptor, ... )	static auto Method = g_Java.GetEnv()->GetMethodID( GetThisClass(), Name, Descriptor ); \
												return g_Java.GetEnv()->CallIntMethod( this, Method, __VA_ARGS__ )

#define CALL_FLOATMETHOD( Name, Descriptor, ... )	static auto Method = g_Java.GetEnv()->GetMethodID( GetThisClass(), Name, Descriptor ); \
													return g_Java.GetEnv()->CallFloatMethod( this, Method, __VA_ARGS__ )

#define CALL_DOUBLEMETHOD( Name, Descriptor, ... )	static auto Method = g_Java.GetEnv()->GetMethodID( GetThisClass(), Name, Descriptor ); \
													return g_Java.GetEnv()->CallDoubleMethod( this, Method, __VA_ARGS__ )

#define CALL_STATICOBJECTMETHOD( ReturnType, Name, Descriptor, ... )	static auto Method = g_Java.GetEnv()->GetStaticMethodID( GetThisClass(), Name, Descriptor ); \
																		return JObjectGuard( reinterpret_cast<ReturnType>( g_Java.GetEnv()->CallStaticObjectMethod( GetThisClass(), Method, __VA_ARGS__ ) ) )

#define CALL_STATICINTMETHOD( Name, Descriptor, ... )	static auto Method = g_Java.GetEnv()->GetStaticMethodID( GetThisClass(), Name, Descriptor ); \
														return g_Java.GetEnv()->CallStaticIntMethod( GetThisClass(), Method, __VA_ARGS__ )

#define GET_OBJECTFIELD( ReturnType, Name, Descriptor )	static auto Field = g_Java.GetEnv()->GetFieldID( GetThisClass(), Name, Descriptor ); \
														return JObjectGuard( reinterpret_cast<ReturnType>( g_Java.GetEnv()->GetObjectField( this, Field ) ) )

#define GET_BOOLEANFIELD( Name )	static auto Field = g_Java.GetEnv()->GetFieldID( GetThisClass(), Name, "Z" ); \
									return g_Java.GetEnv()->GetBooleanField( this, Field )

#define GET_FLOATFIELD( Name )	static auto Field = g_Java.GetEnv()->GetFieldID( GetThisClass(), Name, "F" ); \
								return g_Java.GetEnv()->GetFloatField( this, Field )

#define GET_DOUBLEFIELD( Name )	static auto Field = g_Java.GetEnv()->GetFieldID( GetThisClass(), Name, "D" ); \
								return g_Java.GetEnv()->GetDoubleField( this, Field )

#define SET_OBJECTFIELD( Name, Descriptor, Value )	static auto Field = g_Java.GetEnv()->GetFieldID( GetThisClass(), Name, Descriptor ); \
													return g_Java.GetEnv()->SetObjectField( this, Field, Value )

#define SET_BOOLEANFIELD( Name, Value )	static auto Field = g_Java.GetEnv()->GetFieldID( GetThisClass(), Name, "Z" ); \
										return g_Java.GetEnv()->SetBooleanField( this, Field, Value )

#define SET_DOUBLEFIELD( Name, Value )	static auto Field = g_Java.GetEnv()->GetFieldID( GetThisClass(), Name, "D" ); \
										return g_Java.GetEnv()->SetDoubleField( this, Field, Value )

#define GET_STATICOBJECTFIELD( ReturnType, Name, Descriptor )	static auto Field = g_Java.GetEnv()->GetStaticFieldID( GetThisClass(), Name, Descriptor ); \
																return JObjectGuard( reinterpret_cast<ReturnType>( g_Java.GetEnv()->GetStaticObjectField( GetThisClass(), Field ) ) )