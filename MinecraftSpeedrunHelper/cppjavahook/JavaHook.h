#pragma once

#include <string>
#include <vector>
#include <jvmti.h>
#include "../Memory/Hook.h"

class JavaHook : public Memory::Hook
{
public:
	class HookData
	{
	public:
		explicit HookData( JNIEnv* Env, jobjectArray Arguments, bool IsVoid );

		template <typename T = _jobject>
		T* GetArgument( jsize Index ) const
		{
			if ( !m_IsVoid )
				++Index;

			return reinterpret_cast<T*>( m_Env->GetObjectArrayElement( m_Arguments, Index + 1 ) );
		}
		
		void SetArgument( jsize Index, jobject Value ) const;
		
		void SetReturn( jobject Return ) const;
		
	private:
		static jclass GetBooleanClass( JNIEnv* Env );
		static jmethodID GetBooleanValueOfMethod( JNIEnv* Env );
		
		JNIEnv* m_Env;
		jobjectArray m_Arguments;
		bool m_IsVoid;
	};
	using Handler_t = void( * )( JNIEnv*, const HookData& );
	
	JavaHook();

	void Unhook() override;
	
	void AddHook( const std::string& Class, const std::string& Name, const std::string& Descriptor, Handler_t Handler );

	void Init( JNIEnv* Env, jvmtiEnv* JVMTIEnv );
	void Init( JNIEnv* Env, jvmtiEnv* JVMTIEnv, jvmtiEventCallbacks* Callbacks );

	static void JNICALL ClassFileLoadHook( jvmtiEnv* jvmti_env, JNIEnv* jni_env, jclass class_being_redefined, jobject loader, const char* name, jobject protection_domain, jint class_data_len,
										   const unsigned char* class_data, jint* new_class_data_len, unsigned char** new_class_data );
	
private:
	static jobjectArray JNICALL Handler( JNIEnv* env, jclass obj, jshort MethodID, jobjectArray Arguments );

	std::vector<jclass> GetClasses();
	
	enum class Type // TO DO: All types
	{
		Integer = 'I',
		Boolean = 'Z',
		Object = 'L',
		Void = 'V'
	};
	
	struct NamedType
	{
		NamedType( Type type, const std::string& name ) :
			Type( type ),
			Name( name )
		{ }
		
		Type Type;
		std::string Name;
	};

	static std::vector<NamedType> GetParameters( const std::string_view& Descriptor );
	static NamedType GetReturnType( const std::string_view& Descriptor );
	
	struct Hook
	{
		Hook( const std::string& clazz, const std::string& name, const std::string& descriptor, Handler_t handler ) :
			Class( clazz ),
			Name( name ),
			Descriptor( descriptor ),
			Handler( handler ),
			IsVoid( false )
		{ }

		std::string Class;
		std::string Name;
		std::string Descriptor;
		Handler_t Handler;

		bool IsVoid;
	};
	std::vector<Hook> m_Hooks;

	JNIEnv* m_Env;
	jvmtiEnv* m_JVMTIEnv;
	bool m_Unhook;
	jvmtiEventCallbacks m_OldCallbacks;
	
	static inline JavaHook* s_JavaHook;
};