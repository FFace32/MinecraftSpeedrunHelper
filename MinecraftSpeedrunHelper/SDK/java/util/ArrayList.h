#pragma once

#include <Java.h>

namespace java::util
{
    class ObjectArrayList : public _jobject
    {
    public:
        DEFINE_THISCLASS( "java/util/ArrayList" )
    	
        static auto init()
        {
            CALL_NEWOBJECT( ObjectArrayList*, "()V" );
        }

        jboolean add( jobject e )
        {
            CALL_BOOLEANMETHOD( "add", "(Ljava/lang/Object;)Z", e );
        }

        auto get( jint index )
        {
            CALL_OBJECTMETHOD( jobject, "get", "(I)Ljava/lang/Object;", index );
        }

    	jint size()
        {
            CALL_INTMETHOD( "size", "()I" );
        }
    };
	
	template <typename T>
    class ArrayList : public ObjectArrayList
    {
    public:
        using BaseClass = ObjectArrayList;
        using ThisT = ArrayList<T>;
		
        static JObjectGuard<ThisT*> init()
        {
            return BaseClass::init().CastTo<ThisT*>();
        }

        jboolean add( const JObjectGuard<T*>& e )
        {
            return BaseClass::add( e );
        }

		JObjectGuard<T*> get( jint index )
        {
            return BaseClass::get( index ).CastTo<T*>();
        }

        class Iterator
        {
        public:
            explicit Iterator( ThisT* Parent, jint Index ) :
                m_Parent( Parent ),
                m_Index( Index )
            { }

            Iterator operator++()
	        {
                ++m_Index;
	        	
                return *this;
	        }

            bool operator!=( const Iterator& Rhs ) const
	        {
                return m_Index != Rhs.m_Index;
	        }

            JObjectGuard<T*> operator*() const
	        {
                return m_Parent->get( m_Index );
	        }
        	
        private:
            ThisT* m_Parent;
            jint m_Index;
        };
        Iterator begin()
        {
            return Iterator( this, 0 );
        }
        Iterator end()
        {
            return Iterator( this, size() );
        }
    };
}