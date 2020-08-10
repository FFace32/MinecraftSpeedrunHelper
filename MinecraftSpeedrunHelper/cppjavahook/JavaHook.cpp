#include "JavaHook.h"
#include <algorithm>
#include <stdexcept>

// I assume ClassFileLoadHook won't give me bad data so the buffer isn't checked at all
class ClassFile
{
public:
    static const auto InvalidIndex = static_cast<uint16_t>( -1 );

    struct attribute_info
    {
        uint16_t attribute_name_index;
        std::vector<uint8_t> info;
    };

    struct method_info
    {
        uint16_t access_flags;
        uint16_t name_index;
        uint16_t descriptor_index;
        std::vector<attribute_info> attributes;

        uint16_t Index;
    };

    explicit ClassFile( const uint8_t* Buffer ) :
        m_Buffer( Buffer ),
        m_WriteOffset( 0 )
    {
        m_BufferSize = sizeof( uint32_t ) + sizeof( uint16_t ) * 2;

        uint16_t Count;
        Read( Count );

        m_OriginalConstantPoolStart = m_BufferSize;

        m_ConstantPool.resize( Count - 1 );
        for ( auto i = 0; i < Count - 1; ++i )
        {
            auto& Info = m_ConstantPool[i];
            Read( Info.tag );

            uint16_t Size;
            switch ( Info.tag )
            {
            case CONSTANT_Utf8:
                Read( Size );

                break;
            case CONSTANT_Integer:
            case CONSTANT_Float:
                Size = sizeof( uint32_t );

                break;
            case CONSTANT_Long:
            case CONSTANT_Double:
                Size = sizeof( uint64_t );
                m_ConstantPool[++i].tag = 0;

                break;
            case CONSTANT_Class:
            case CONSTANT_String:
            case CONSTANT_MethodType:
                Size = sizeof( uint16_t );

                break;
            case CONSTANT_Fieldref:
            case CONSTANT_Methodref:
            case CONSTANT_InterfaceMethodref:
            case CONSTANT_NameAndType:
            case CONSTANT_InvokeDynamic:
                Size = sizeof( uint16_t ) * 2;

                break;
            case CONSTANT_MethodHandle:
                Size = sizeof( uint8_t ) + sizeof( uint16_t );

                break;
            default:
                throw "";
            }

            Info.info.resize( Size );
            memcpy( Info.info.data(), m_Buffer + m_BufferSize, Size );
            m_BufferSize += Size;
        }

        m_OriginalConstantPoolEnd = m_BufferSize;

        m_BufferSize += sizeof( uint16_t ) * 3;

        Read( Count );
        m_BufferSize += sizeof( uint16_t ) * Count;

        Read( Count );
        for ( auto i = 0; i < Count; ++i )
        {
            m_BufferSize += sizeof( uint16_t ) * 3;

            uint16_t AttributesCount;
            Read( AttributesCount );

            for ( auto j = 0; j < AttributesCount; ++j )
            {
                m_BufferSize += sizeof( uint16_t );

                uint32_t AttributeLength;
                Read( AttributeLength );

                m_BufferSize += sizeof( uint8_t ) * AttributeLength;
            }
        }

        Read( Count );

        m_OriginalMethodsStart = m_BufferSize;

        m_Methods.resize( Count );
        for ( auto i = 0; i < Count; ++i )
        {
            auto& Method = m_Methods[i];
            Read( Method.access_flags );
            Read( Method.name_index );
            Read( Method.descriptor_index );

            Method.Index = i;

            uint16_t AttributesCount;
            Read( AttributesCount );

            Method.attributes.resize( AttributesCount );
            for ( auto j = 0; j < AttributesCount; ++j )
            {
                auto& Attribute = Method.attributes[j];
                Read( Attribute.attribute_name_index );

                uint32_t AttributeLength;
                Read( AttributeLength );

                if ( GetUTF8( Attribute.attribute_name_index ) == "Code" )
                {
                    Attribute.info.resize( sizeof( Code_attribute ) );

                    auto& Code = *new ( Attribute.info.data() ) Code_attribute(); // This feels really hacky
                    Read( Code.max_stack );
                    Read( Code.max_locals );

                    uint32_t CodeLength;
                    Read( CodeLength );

                    Code.code.resize( CodeLength );
                    memcpy( Code.code.data(), m_Buffer + m_BufferSize, CodeLength );
                    m_BufferSize += CodeLength;

                    uint16_t ExceptionTableLength;
                    Read( ExceptionTableLength );

                    Code.exception_table.resize( ExceptionTableLength );
                    for ( auto q = 0; q < ExceptionTableLength; ++q )
                    {
                        auto& Exception = Code.exception_table[q];
                        Read( Exception.start_pc );
                        Read( Exception.end_pc );
                        Read( Exception.handler_pc );
                        Read( Exception.catch_type );
                    }

                    uint16_t CodeAttributesCount;
                    Read( CodeAttributesCount );

                    Code.attributes.resize( CodeAttributesCount );
                    for ( auto q = 0; q < CodeAttributesCount; ++q )
                    {
                        auto& CodeAttribute = Code.attributes[q];
                        Read( CodeAttribute.attribute_name_index );

                        uint32_t CodeAttributeLength;
                        Read( CodeAttributeLength );

                        if ( GetUTF8( CodeAttribute.attribute_name_index ) == "StackMapTable" )
                        {
                            CodeAttribute.info.resize( sizeof( StackMapTable_attribute ) );

                            auto& StackMapTable = *new ( CodeAttribute.info.data() ) StackMapTable_attribute(); // This feels really hacky

                            uint16_t EntriesCount;
                            Read( EntriesCount );

                            StackMapTable.entries.resize( EntriesCount );
                            for ( auto k = 0; k < EntriesCount; ++k )
                            {
                                auto& Entry = StackMapTable.entries[k];

                                Read( Entry.frame_type );

                                static const auto GetInfo = []( ClassFile* thisptr )
                                {
                                    verification_type_info Info;
                                    thisptr->Read( Info.tag );

                                    if ( Info.tag == 7 )
                                        thisptr->Read( Info.cpool_index );
                                    else if ( Info.tag == 8 )
                                        thisptr->Read( Info.offset );

                                    return Info;
                                };

                                if ( Entry.frame_type >= 64 && Entry.frame_type <= 127 )
                                    Entry.stack.emplace_back( GetInfo( this ) );
                                else if ( Entry.frame_type == 247 )
                                {
                                    Read( Entry.offset_delta );
                                    Entry.stack.emplace_back( GetInfo( this ) );
                                }
                                else if ( Entry.frame_type >= 248 && Entry.frame_type <= 251 )
                                    Read( Entry.offset_delta );
                                else if ( Entry.frame_type >= 252 && Entry.frame_type <= 254 )
                                {
                                    Read( Entry.offset_delta );
                                    for ( auto l = 0; l < Entry.frame_type - 251; ++l )
                                        Entry.locals.emplace_back( GetInfo( this ) );
                                }
                                else if ( Entry.frame_type == 255 )
                                {
                                    Read( Entry.offset_delta );

                                    uint16_t LocalsCount;
                                    Read( LocalsCount );

                                    Entry.locals.resize( LocalsCount );
                                    for ( auto l = 0; l < LocalsCount; ++l )
                                        Entry.locals[l] = GetInfo( this );

                                    uint16_t StackCount;
                                    Read( StackCount );

                                    Entry.stack.resize( StackCount );
                                    for ( auto l = 0; l < StackCount; ++l )
                                        Entry.stack[l] = GetInfo( this );
                                }
                                // throw if [128-246]?
                            }
                        }
                        else
                        {
                            CodeAttribute.info.resize( CodeAttributeLength );
                            memcpy( CodeAttribute.info.data(), m_Buffer + m_BufferSize, CodeAttributeLength );
                            m_BufferSize += CodeAttributeLength;
                        }
                    }
                }
                else
                {
                    Attribute.info.resize( AttributeLength );
                    memcpy( Attribute.info.data(), m_Buffer + m_BufferSize, AttributeLength );
                    m_BufferSize += AttributeLength;
                }
            }
        }

        m_OriginalMethodsEnd = m_BufferSize;

        Read( Count );
        for ( auto i = 0; i < Count; ++i )
        {
            m_BufferSize += sizeof( uint16_t );

            uint32_t AttributeLength;
            Read( AttributeLength );

            m_BufferSize += sizeof( uint8_t ) * AttributeLength;
        }
    }

    void CopyTo( uint8_t* Buffer )
    {
        m_WriteOffset = 0;

        Write( Buffer, m_Buffer, sizeof( uint32_t ) + sizeof( uint16_t ) * 2 );

        Write( Buffer, static_cast<uint16_t>( m_ConstantPool.size() + 1 ) );
        for ( const auto& Info : m_ConstantPool )
        {
            if ( Info.tag )
            {
                Write( Buffer, Info.tag );

                if ( Info.tag == CONSTANT_Utf8 )
                    Write( Buffer, static_cast<uint16_t>( Info.info.size() ) );

                Write( Buffer, Info.info.data(), Info.info.size() );
            }
        }

        Write( Buffer, m_Buffer + m_OriginalConstantPoolEnd, m_OriginalMethodsStart - m_OriginalConstantPoolEnd - sizeof( uint16_t ) );

        Write( Buffer, static_cast<uint16_t>( m_Methods.size() ) );
        for ( const auto& Method : m_Methods )
        {
            Write( Buffer, Method.access_flags );
            Write( Buffer, Method.name_index );
            Write( Buffer, Method.descriptor_index );
            Write( Buffer, static_cast<uint16_t>( Method.attributes.size() ) );

            for ( const auto& Attribute : Method.attributes )
            {
                Write( Buffer, Attribute.attribute_name_index );

                if ( GetUTF8( Attribute.attribute_name_index ) == "Code" )
                {
                    auto& Code = *reinterpret_cast<const Code_attribute*>( Attribute.info.data() );
                    Write( Buffer, GetCodeAttributeLength( Code ) );

                    Write( Buffer, Code.max_stack );
                    Write( Buffer, Code.max_locals );
                    Write( Buffer, static_cast<uint32_t>( Code.code.size() ) );
                    Write( Buffer, Code.code.data(), Code.code.size() );

                    Write( Buffer, static_cast<uint16_t>( Code.exception_table.size() ) );
                    for ( const auto& Exception : Code.exception_table )
                    {
                        Write( Buffer, Exception.start_pc );
                        Write( Buffer, Exception.end_pc );
                        Write( Buffer, Exception.handler_pc );
                        Write( Buffer, Exception.catch_type );
                    }

                    Write( Buffer, static_cast<uint16_t>( Code.attributes.size() ) );
                    for ( const auto& CodeAttribute : Code.attributes )
                    {
                        Write( Buffer, CodeAttribute.attribute_name_index );

                        if ( GetUTF8( CodeAttribute.attribute_name_index ) == "StackMapTable" )
                        {
                            auto& StackMapTable = *reinterpret_cast<const StackMapTable_attribute*>( CodeAttribute.info.data() );
                            Write( Buffer, GetStackMapTableLength( StackMapTable ) );

                            Write( Buffer, static_cast<uint16_t>( StackMapTable.entries.size() ) );
                            for ( const auto& Entry : StackMapTable.entries )
                            {
                                Write( Buffer, Entry.frame_type );

                                static const auto SetInfo = []( ClassFile* thisptr, uint8_t* Buffer, const verification_type_info& Info )
                                {
                                    thisptr->Write( Buffer, Info.tag );
                                    if ( Info.tag == 7 )
                                        thisptr->Write( Buffer, Info.cpool_index );
                                    else if ( Info.tag == 8 )
                                        thisptr->Write( Buffer, Info.offset );
                                };

                                if ( Entry.frame_type >= 64 && Entry.frame_type <= 127 )
                                    SetInfo( this, Buffer, Entry.stack[0] );
                                else if ( Entry.frame_type == 247 )
                                {
                                    Write( Buffer, Entry.offset_delta );
                                    SetInfo( this, Buffer, Entry.stack[0] );
                                }
                                else if ( Entry.frame_type >= 248 && Entry.frame_type <= 251 )
                                {
                                    Write( Buffer, Entry.offset_delta );
                                }
                                else if ( Entry.frame_type >= 252 && Entry.frame_type <= 254 )
                                {
                                    Write( Buffer, Entry.offset_delta );
                                    for ( const auto& Local : Entry.locals )
                                        SetInfo( this, Buffer, Local );
                                }
                                else if ( Entry.frame_type == 255 )
                                {
                                    Write( Buffer, Entry.offset_delta );

                                    Write( Buffer, static_cast<uint16_t>( Entry.locals.size() ) );
                                    for ( const auto& Local : Entry.locals )
                                        SetInfo( this, Buffer, Local );

                                    Write( Buffer, static_cast<uint16_t>( Entry.stack.size() ) );
                                    for ( const auto& Stack : Entry.stack )
                                        SetInfo( this, Buffer, Stack );
                                }
                            }
                        }
                        else
                        {
                            Write( Buffer, static_cast<uint32_t>( CodeAttribute.info.size() ) );
                            Write( Buffer, CodeAttribute.info.data(), CodeAttribute.info.size() );
                        }
                    }
                }
                else
                {
                    Write( Buffer, static_cast<uint32_t>( Attribute.info.size() ) );
                    Write( Buffer, Attribute.info.data(), Attribute.info.size() );
                }
            }
        }

        Write( Buffer, m_Buffer + m_OriginalMethodsEnd, m_BufferSize - m_OriginalMethodsEnd );
    }

    size_t GetSize() const
    {
        return m_BufferSize - ( m_OriginalConstantPoolEnd - m_OriginalConstantPoolStart ) - ( m_OriginalMethodsEnd - m_OriginalMethodsStart ) + GetConstantPoolSize() + GetMethodsSize();
    }

    std::string_view GetUTF8( uint16_t Index ) const
    {
        if ( Index > m_ConstantPool.size() )
            return nullptr;

        auto& Return = m_ConstantPool[Index - 1];
        if ( Return.tag != CONSTANT_Utf8 )
            return nullptr;

        return std::string_view( reinterpret_cast<const char*>( Return.info.data() ), Return.info.size() );
    }

    uint16_t FindUTF8( const std::string_view& UTF8 ) const
    {
        auto Find = std::find_if( m_ConstantPool.begin(), m_ConstantPool.end(), [&UTF8]( const cp_info& Info )
        {
            return Info.tag == CONSTANT_Utf8 && std::string_view( reinterpret_cast<const char*>( Info.info.data() ), Info.info.size() ) == UTF8;
        } );
        if ( Find == m_ConstantPool.end() )
            return InvalidIndex;

        return static_cast<uint16_t>( std::distance( m_ConstantPool.begin(), Find ) + 1 );
    }

    uint16_t FindOrAddUTF8( const std::string_view& UTF8 )
    {
        auto Find = FindUTF8( UTF8 );
        if ( Find == InvalidIndex )
        {
            cp_info Info;
            Info.tag = CONSTANT_Utf8;

            Info.info.resize( UTF8.length() );
            memcpy( Info.info.data(), UTF8.data(), UTF8.length() );

            m_ConstantPool.emplace_back( Info );

            return static_cast<uint16_t>( m_ConstantPool.size() );
        }

        return Find;
    }

    uint16_t FindOrAddClass( uint16_t Name )
    {
        swap_endian( Name );

        auto Find = std::find_if( m_ConstantPool.begin(), m_ConstantPool.end(), [Name]( const cp_info& Info )
        {
            return Info.tag == CONSTANT_Class && *reinterpret_cast<const uint16_t*>( Info.info.data() ) == Name;
        } );
        if ( Find == m_ConstantPool.end() )
        {
            cp_info Info;
            Info.tag = CONSTANT_Class;

            Info.info.resize( sizeof( uint16_t ) );
            memcpy( Info.info.data(), &Name, sizeof( uint16_t ) );

            m_ConstantPool.emplace_back( Info );

            return static_cast<uint16_t>( m_ConstantPool.size() );
        }

        return static_cast<uint16_t>( std::distance( m_ConstantPool.begin(), Find ) + 1 );
    }

    uint16_t FindOrAddInteger( uint32_t Integer )
    {
        swap_endian( Integer );

        auto Find = std::find_if( m_ConstantPool.begin(), m_ConstantPool.end(), [Integer]( const cp_info& Info )
        {
            return Info.tag == CONSTANT_Integer && *reinterpret_cast<const uint32_t*>( Info.info.data() ) == Integer;
        } );
        if ( Find == m_ConstantPool.end() )
        {
            cp_info Info;
            Info.tag = CONSTANT_Class;

            Info.info.resize( sizeof( uint32_t ) );
            memcpy( Info.info.data(), &Integer, sizeof( uint32_t ) );

            m_ConstantPool.emplace_back( Info );

            return static_cast<uint16_t>( m_ConstantPool.size() );
        }

        return static_cast<uint16_t>( std::distance( m_ConstantPool.begin(), Find ) + 1 );
    }

    uint16_t FindOrAddNameAndType( uint16_t Name, uint16_t Type )
    {
        swap_endian( Name );
        swap_endian( Type );

        auto Find = std::find_if( m_ConstantPool.begin(), m_ConstantPool.end(), [Name, Type]( const cp_info& Info )
        {
            return Info.tag == CONSTANT_NameAndType && *reinterpret_cast<const uint16_t*>( Info.info.data() ) == Name && *reinterpret_cast<const uint16_t*>( Info.info.data() + sizeof( uint16_t ) ) == Type;
        } );
        if ( Find == m_ConstantPool.end() )
        {
            cp_info Info;
            Info.tag = CONSTANT_NameAndType;

            Info.info.resize( sizeof( uint16_t ) * 2 );
            memcpy( Info.info.data(), &Name, sizeof( uint16_t ) );
            memcpy( Info.info.data() + sizeof( uint16_t ), &Type, sizeof( uint16_t ) );

            m_ConstantPool.emplace_back( Info );

            return static_cast<uint16_t>( m_ConstantPool.size() );
        }

        return static_cast<uint16_t>( std::distance( m_ConstantPool.begin(), Find ) + 1 );
    }

    uint16_t FindOrAddMethodRef( uint16_t Class, uint16_t NameAndType )
    {
        swap_endian( Class );
        swap_endian( NameAndType );

        auto Find = std::find_if( m_ConstantPool.begin(), m_ConstantPool.end(), [Class, NameAndType]( const cp_info& Info )
        {
            return Info.tag == CONSTANT_Methodref && *reinterpret_cast<const uint16_t*>( Info.info.data() ) == Class && *reinterpret_cast<const uint16_t*>( Info.info.data() + sizeof( uint16_t ) ) == NameAndType;
        } );
        if ( Find == m_ConstantPool.end() )
        {
            cp_info Info;
            Info.tag = CONSTANT_Methodref;

            Info.info.resize( sizeof( uint16_t ) * 2 );
            memcpy( Info.info.data(), &Class, sizeof( uint16_t ) );
            memcpy( Info.info.data() + sizeof( uint16_t ), &NameAndType, sizeof( uint16_t ) );

            m_ConstantPool.emplace_back( Info );

            return static_cast<uint16_t>( m_ConstantPool.size() );
        }

        return static_cast<uint16_t>( std::distance( m_ConstantPool.begin(), Find ) + 1 );
    }

    method_info* FindMethod( const std::string_view& Name, const std::string_view& Descriptor )
    {
        auto NameIndex = FindUTF8( Name );
        if ( NameIndex == InvalidIndex )
            return nullptr;

        auto DescriptorIndex = FindUTF8( Descriptor );
        if ( DescriptorIndex == InvalidIndex )
            return nullptr;

        for ( auto& Method : m_Methods )
        {
            if ( Method.name_index == NameIndex && Method.descriptor_index == DescriptorIndex )
                return &Method;
        }

        return nullptr;
    }

    uint16_t FindMethodAttribute( uint16_t MethodIndex, const std::string_view& Name )
    {
        auto NameIndex = FindUTF8( Name );
        if ( NameIndex == InvalidIndex )
            return InvalidIndex;

        const auto& Method = m_Methods[MethodIndex];
        for ( uint16_t i = 0; i < static_cast<uint16_t>( Method.attributes.size() ); ++i )
        {
            if ( Method.attributes[i].attribute_name_index == NameIndex )
                return i;
        }

        return InvalidIndex;
    }

    //private:
    enum
    {
        CONSTANT_Utf8 = 1,
        CONSTANT_Integer = 3,
        CONSTANT_Float,
        CONSTANT_Long,
        CONSTANT_Double,
        CONSTANT_Class,
        CONSTANT_String,
        CONSTANT_Fieldref,
        CONSTANT_Methodref,
        CONSTANT_InterfaceMethodref,
        CONSTANT_NameAndType,
        CONSTANT_MethodHandle = 15,
        CONSTANT_MethodType,
        CONSTANT_InvokeDynamic = 18,
    };

    enum
    {
        ACC_PUBLIC = 0x0001,
        ACC_PRIVATE = 0x0002,
        ACC_PROTECTED = 0x0004,
        ACC_STATIC = 0x0008,
        ACC_FINAL = 0x0010,
        ACC_SUPER_ACC_SYNCHRONIZED = 0x0020,
        ACC_BRIDGE_ACC_VOLATILE = 0x0040,
        ACC_VARARGS_ACC_TRANSIENT = 0x0080,
        ACC_NATIVE = 0x0100,
        ACC_INTERFACE = 0x0200,
        ACC_ABSTRACT = 0x0400,
        ACC_STRICT = 0x0800,
        ACC_SYNTHETIC = 0x1000,
        ACC_ANNOTATION = 0x2000,
        ACC_ENUM = 0x4000
    };

    size_t GetConstantPoolSize() const
    {
        size_t Size = 0;
        for ( const auto& Info : m_ConstantPool )
        {
            if ( !Info.tag )
                continue;

            Size += sizeof( uint8_t ) + Info.info.size();

            if ( Info.tag == CONSTANT_Utf8 )
                Size += sizeof( uint16_t );
        }

        return Size;
    }

    size_t GetMethodsSize() const
    {
        size_t Size = 0;
        for ( const auto& Method : m_Methods )
        {
            Size += sizeof( uint16_t ) * 4;
            for ( const auto& Attribute : Method.attributes )
                Size += sizeof( uint16_t ) + sizeof( uint32_t ) + GetAttributeLength( Attribute );
        }

        return Size;
    }

    template <typename T>
    void Read( T& Variable )
    {
        Variable = *reinterpret_cast<const T*>( m_Buffer + m_BufferSize );
        m_BufferSize += sizeof( T );
        swap_endian( Variable );
    }

    template <typename T>
    void Write( uint8_t* Buffer, const T& Variable )
    {
        auto Copy = Variable;
        swap_endian( Copy );
        Write( Buffer, reinterpret_cast<const uint8_t*>( &Copy ), sizeof( T ) );
    }

    void Write( uint8_t* To, const uint8_t* From, size_t Size )
    {
        memcpy( To + m_WriteOffset, From, Size );
        m_WriteOffset += Size;
    }

    const uint8_t* m_Buffer;
    size_t m_BufferSize;

    struct cp_info
    {
        uint8_t tag;
        std::vector<uint8_t> info;
    };
    std::vector<cp_info> m_ConstantPool;
    size_t m_OriginalConstantPoolStart;
    size_t m_OriginalConstantPoolEnd;

    std::vector<method_info> m_Methods;
    size_t m_OriginalMethodsStart;
    size_t m_OriginalMethodsEnd;

    size_t m_WriteOffset;

    //
    template <typename T>
    static void swap_endian( T& u )
    {
        union
        {
            T u;
            unsigned char u8[sizeof( T )];
        } source, dest;

        source.u = u;

        for ( size_t k = 0; k < sizeof( T ); k++ )
            dest.u8[k] = source.u8[sizeof( T ) - k - 1];

        u = dest.u;
    }

    struct verification_type_info
    {
        uint8_t tag;

        union
        {
            uint16_t cpool_index;
            uint16_t offset;
        };
    };

    struct stack_map_frame
    {
        uint8_t frame_type;
        uint16_t offset_delta;
        std::vector<verification_type_info> locals;
        std::vector<verification_type_info> stack;
    };

    struct StackMapTable_attribute
    {
        std::vector<stack_map_frame> entries;
    };

    struct Code_attribute
    {
        uint16_t max_stack;
        uint16_t max_locals;
        std::vector<uint8_t> code;

        struct exception_table_t
        {
            uint16_t start_pc;
            uint16_t end_pc;
            uint16_t handler_pc;
            uint16_t catch_type;
        };
        std::vector<exception_table_t> exception_table;
        std::vector<attribute_info> attributes;

        using Iterator = std::vector<uint8_t>::iterator;
        Iterator Insert( const std::vector<uint8_t>& Code, const Iterator& At )
        {
            return code.insert( At, Code.begin(), Code.end() ) + Code.size();
        }

        Iterator Ldc( uint16_t Index, const Iterator& At )
        {
            if ( Index <= std::numeric_limits<std::uint8_t>::max() )
                return Insert( { 0x12, static_cast<uint8_t>( Index ) }, At ); // ldc Index

            return Insert( { 0x13, static_cast<uint8_t>( Index >> 8 ), static_cast<uint8_t>( Index ) }, At ); // ldc_w Index
        }

        Iterator Dup( const Iterator& At )
        {
            return Insert( { 0x59 }, At ); // dup
        }

        Iterator AALoad( const Iterator& At )
        {
            return Insert( { 0x32 }, At ); // aaload
        }

        Iterator AAStore( const Iterator& At )
        {
            return Insert( { 0x53 }, At ); // aastore
        }

        Iterator AReturn( const Iterator& At )
        {
            return Insert( { 0xB0 }, At ); // areturn
        }

        Iterator Return( const Iterator& At )
        {
            return Insert( { 0xB1 }, At ); // return
        }

        Iterator CheckCast( uint16_t Index, const Iterator& At )
        {
            return Insert( { 0xC0, static_cast<uint8_t>( Index >> 8 ), static_cast<uint8_t>( Index ) }, At ); // checkcast Index
        }

        Iterator IfNE( uint16_t Offset, const Iterator& At )
        {
            return Insert( { 0x9A, static_cast<uint8_t>( Offset >> 8 ), static_cast<uint8_t>( Offset ) }, At ); // ifne Offset
        }

        Iterator ANewArray( uint16_t Type, const Iterator& At )
        {
            return Insert( { 0xBD, static_cast<uint8_t>( Type >> 8 ), static_cast<uint8_t>( Type ) }, At ); // anewarray Type
        }

        Iterator InvokeVirtual( uint16_t Index, const Iterator& At )
        {
            return Insert( { 0xB6, static_cast<uint8_t>( Index >> 8 ), static_cast<uint8_t>( Index ) }, At ); // invokevirtual Index
        }

        Iterator InvokeStatic( uint16_t Index, const Iterator& At )
        {
            return Insert( { 0xB8, static_cast<uint8_t>( Index >> 8 ), static_cast<uint8_t>( Index ) }, At ); // invokestatic Index
        }

        Iterator ILoad( uint16_t Index, const Iterator& At ) // Shouldn't Index be 32bit?
        {
            if ( Index <= 3 )
                return Insert( { static_cast<uint8_t>( 0x1A + Index ) }, At ); // iload_0-iload_3

            if ( Index <= std::numeric_limits<std::uint8_t>::max() )
                return Insert( { 0x15, static_cast<uint8_t>( Index ) }, At ); // iload Index

            return Insert( { 0xC4, 0x15, static_cast<uint8_t>( Index >> 8 ), static_cast<uint8_t>( Index ) }, At ); // wide iload Index
        }

        Iterator IStore( uint16_t Index, const Iterator& At ) // Shouldn't Index be 32bit?
        {
            if ( Index <= 3 )
                return Insert( { static_cast<uint8_t>( 0x3B + Index ) }, At ); // istore_0-istore_3

            if ( Index <= std::numeric_limits<std::uint8_t>::max() )
                return Insert( { 0x36, static_cast<uint8_t>( Index ) }, At ); // istore Index

            return Insert( { 0xC4, 0x36, static_cast<uint8_t>( Index >> 8 ), static_cast<uint8_t>( Index ) }, At ); // wide istore Index
        }

        Iterator ALoad( uint16_t Index, const Iterator& At ) // Shouldn't Index be 32bit?
        {
            if ( Index <= 3 )
                return Insert( { static_cast<uint8_t>( 0x2A + Index ) }, At ); // aload_0-aload_3

            if ( Index <= std::numeric_limits<std::uint8_t>::max() )
                return Insert( { 0x19, static_cast<uint8_t>( Index ) }, At ); // aload Index

            return Insert( { 0xC4, 0x19, static_cast<uint8_t>( Index >> 8 ), static_cast<uint8_t>( Index ) }, At ); // wide aload Index
        }

        Iterator AStore( uint16_t Index, const Iterator& At ) // Shouldn't Index be 32bit?
        {
            if ( Index <= 3 )
                return Insert( { static_cast<uint8_t>( 0x4B + Index ) }, At ); // astore_0-astore_3

            if ( Index <= std::numeric_limits<std::uint8_t>::max() )
                return Insert( { 0x3A, static_cast<uint8_t>( Index ) }, At ); // astore Index

            return Insert( { 0xC4, 0x3A, static_cast<uint8_t>( Index >> 8 ), static_cast<uint8_t>( Index ) }, At ); // wide astore Index
        }

        Iterator PushShort( uint16_t Short, const Iterator& At )
        {
            if ( Short <= 5 )
                return Insert( { static_cast<uint8_t>( 0x03 + Short ) }, At ); // iconst_0-iconst_5

            if ( Short <= std::numeric_limits<std::uint8_t>::max() )
                return Insert( { 0x10, static_cast<uint8_t>( Short ) }, At ); // bipush Short

            return Insert( { 0x11, static_cast<uint8_t>( Short >> 8 ), static_cast<uint8_t>( Short ) }, At ); // sipush Short
        }

        Iterator PushInt( ClassFile& ClassFile, uint32_t Int, const Iterator& At )
        {
            if ( Int <= std::numeric_limits<std::uint16_t>::max() )
                return PushShort( Int, At );

            return Ldc( ClassFile.FindOrAddInteger( Int ), At );
        }

        attribute_info* GetAttribute( ClassFile& ClassFile, const std::string_view& Name )
        {
            for ( auto& Attribute : attributes )
            {
                if ( ClassFile.GetUTF8( Attribute.attribute_name_index ) == Name )
                    return &Attribute;
            }

            return nullptr;
        }

        StackMapTable_attribute& FindOrCreateStackMapTableOnly( ClassFile& ClassFile )
        {
            StackMapTable_attribute* Return = nullptr;

            auto StackMapTable = ClassFile.FindOrAddUTF8( "StackMapTable" );
            for ( auto it = attributes.begin(); it != attributes.end(); )
            {
                if ( it->attribute_name_index == StackMapTable )
                {
                    Return = reinterpret_cast<StackMapTable_attribute*>( it->info.data() );
                    ++it;
                }
                else
                    it = attributes.erase( it );
            }

            if ( !Return )
            {
                attribute_info Attribute;
                Attribute.attribute_name_index = StackMapTable;

                Attribute.info.resize( sizeof( StackMapTable_attribute ) );
                new ( Attribute.info.data() ) StackMapTable_attribute(); // This feels really hacky

                Return = reinterpret_cast<StackMapTable_attribute*>( attributes.emplace_back( Attribute ).info.data() );
            }

            return *Return;
        }
    };

    uint32_t GetCodeAttributeLength( const Code_attribute& Attribute ) const
    {
        auto AttributeLength = static_cast<uint32_t>( sizeof( uint16_t ) * 4 + sizeof( uint32_t ) + Attribute.code.size() + sizeof( Code_attribute::exception_table_t ) * Attribute.exception_table.size() );
        for ( const auto& CodeAttribute : Attribute.attributes )
        {
            AttributeLength += sizeof( uint16_t ) + sizeof( uint32_t );

            if ( GetUTF8( CodeAttribute.attribute_name_index ) == "StackMapTable" )
                AttributeLength += GetStackMapTableLength( *reinterpret_cast<const StackMapTable_attribute*>( CodeAttribute.info.data() ) );
            else
                AttributeLength += CodeAttribute.info.size();
        }

        return AttributeLength;
    }

    static uint32_t GetStackMapTableLength( const StackMapTable_attribute& Attribute )
    {
        auto AttributeLength = static_cast<uint32_t>( sizeof( uint16_t ) + sizeof( uint8_t ) * Attribute.entries.size() );
        for ( const auto& Entry : Attribute.entries )
        {
            static const auto GetInfoSize = []( const verification_type_info& Info )
            {
                if ( Info.tag == 7 || Info.tag == 8 )
                    return sizeof( uint8_t ) + sizeof( uint16_t );

                return sizeof( uint8_t );
            };

            if ( Entry.frame_type >= 64 && Entry.frame_type <= 127 )
                AttributeLength += GetInfoSize( Entry.stack[0] );
            else if ( Entry.frame_type == 247 )
                AttributeLength += sizeof( uint16_t ) + GetInfoSize( Entry.stack[0] );
            else if ( Entry.frame_type >= 248 && Entry.frame_type <= 251 )
                AttributeLength += sizeof( uint16_t );
            else if ( Entry.frame_type >= 252 && Entry.frame_type <= 254 )
            {
                AttributeLength += sizeof( uint16_t );
                for ( const auto& Local : Entry.locals )
                    AttributeLength += GetInfoSize( Local );
            }
            else if ( Entry.frame_type == 255 )
            {
                AttributeLength += sizeof( uint16_t ) * 3;

                for ( const auto& Local : Entry.locals )
                    AttributeLength += GetInfoSize( Local );

                for ( const auto& Stack : Entry.stack )
                    AttributeLength += GetInfoSize( Stack );
            }
        }

        return AttributeLength;
    }

    uint32_t GetAttributeLength( const attribute_info& Attribute ) const
    {
        if ( GetUTF8( Attribute.attribute_name_index ) == "Code" )
            return GetCodeAttributeLength( *reinterpret_cast<const Code_attribute*>( Attribute.info.data() ) );

        return Attribute.info.size();
    }

    Code_attribute* GetCodeAttribute( method_info* Method )
    {
        auto Attribute = FindMethodAttribute( Method->Index, "Code" );
        if ( Attribute == InvalidIndex )
            return nullptr;

        return reinterpret_cast<Code_attribute*>( m_Methods[Method->Index].attributes[Attribute].info.data() );
    }
};

JavaHook::JavaHook() :
	m_Env( nullptr ),
	m_JVMTIEnv( nullptr ),
	m_Unhook( false ),
	m_OldCallbacks{ }
{
	if ( s_JavaHook )
		throw std::runtime_error( "Only one JavaHook instance allowed" );

	s_JavaHook = this;
}

void JavaHook::Unhook()
{
	m_Unhook = true;
	
	auto Classes = GetClasses();
	m_JVMTIEnv->RetransformClasses( Classes.size(), Classes.data() );
	m_JVMTIEnv->SetEventCallbacks( &m_OldCallbacks, sizeof( m_OldCallbacks ) );
}

void JavaHook::AddHook( const std::string& Class, const std::string& Name, const std::string& Descriptor, Handler_t Handler )
{
	m_Hooks.emplace_back( Class, Name, Descriptor, Handler );
}

void JavaHook::Init( JNIEnv* Env, jvmtiEnv* JVMTIEnv )
{
	jvmtiEventCallbacks Callbacks;
	memset( &Callbacks, 0, sizeof( Callbacks ) );
	Init( Env, JVMTIEnv, &Callbacks );
}

void JavaHook::Init( JNIEnv* Env, jvmtiEnv* JVMTIEnv, jvmtiEventCallbacks* Callbacks )
{
	m_Env = Env;
	m_JVMTIEnv = JVMTIEnv;
	memcpy( &m_OldCallbacks, Callbacks, sizeof( m_OldCallbacks ) );
	
	jvmtiCapabilities Capabilities;
	JVMTIEnv->GetCapabilities( &Capabilities );
	Capabilities.can_retransform_classes = 1;
    if ( auto Error = JVMTIEnv->AddCapabilities( &Capabilities ); Error )
        throw std::runtime_error( "AddCapabilities failed" );

    if ( auto Error = JVMTIEnv->SetEventNotificationMode( JVMTI_ENABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, nullptr ); Error )
        throw std::runtime_error( "SetEventNotificationMode failed" );

	/*
	public class cppjavahook {
		public static native Object[] a( short MethodID, Object... Arguments );
		
		public static Object[] b( short MethodID, Object... Arguments )
		{
			Object[] ArgumentsArray = new Object[Arguments.length + 1];
			ArgumentsArray[0] = new Boolean( true );
			System.arraycopy( Arguments, 0, ArgumentsArray, 1, Arguments.length );
			
			return a( MethodID, ArgumentsArray );
		}
		
		public static Object[] c( short MethodID, Object... Arguments )
		{
			Object[] ArgumentsArray = new Object[Arguments.length + 2];
			ArgumentsArray[0] = new Boolean( true );
			System.arraycopy( Arguments, 0, ArgumentsArray, 2, Arguments.length );
			
			return a( MethodID, ArgumentsArray );
		}
	}
	 */
	const uint8_t ClassData[] = {
		0xCA, 0xFE, 0xBA, 0xBE, 0x00, 0x00, 0x00, 0x34, 0x00, 0x1B, 0x0A, 0x00, 0x02, 0x00, 0x0F, 0x07, 0x00, 0x10, 0x07, 0x00, 0x11, 0x0A, 0x00, 0x03, 0x00, 0x12, 0x0A, 0x00, 0x13, 0x00, 0x14, 0x0A,
		0x00, 0x07, 0x00, 0x15, 0x07, 0x00, 0x16, 0x01, 0x00, 0x06, 0x3C, 0x69, 0x6E, 0x69, 0x74, 0x3E, 0x01, 0x00, 0x03, 0x28, 0x29, 0x56, 0x01, 0x00, 0x04, 0x43, 0x6F, 0x64, 0x65, 0x01, 0x00, 0x01,
		0x61, 0x01, 0x00, 0x29, 0x28, 0x53, 0x5B, 0x4C, 0x6A, 0x61, 0x76, 0x61, 0x2F, 0x6C, 0x61, 0x6E, 0x67, 0x2F, 0x4F, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3B, 0x29, 0x5B, 0x4C, 0x6A, 0x61, 0x76, 0x61,
		0x2F, 0x6C, 0x61, 0x6E, 0x67, 0x2F, 0x4F, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3B, 0x01, 0x00, 0x01, 0x62, 0x01, 0x00, 0x01, 0x63, 0x0C, 0x00, 0x08, 0x00, 0x09, 0x01, 0x00, 0x10, 0x6A, 0x61, 0x76,
		0x61, 0x2F, 0x6C, 0x61, 0x6E, 0x67, 0x2F, 0x4F, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x01, 0x00, 0x11, 0x6A, 0x61, 0x76, 0x61, 0x2F, 0x6C, 0x61, 0x6E, 0x67, 0x2F, 0x42, 0x6F, 0x6F, 0x6C, 0x65, 0x61,
		0x6E, 0x0C, 0x00, 0x08, 0x00, 0x17, 0x07, 0x00, 0x18, 0x0C, 0x00, 0x19, 0x00, 0x1A, 0x0C, 0x00, 0x0B, 0x00, 0x0C, 0x01, 0x00, 0x0B, 0x63, 0x70, 0x70, 0x6A, 0x61, 0x76, 0x61, 0x68, 0x6F, 0x6F,
		0x6B, 0x01, 0x00, 0x04, 0x28, 0x5A, 0x29, 0x56, 0x01, 0x00, 0x10, 0x6A, 0x61, 0x76, 0x61, 0x2F, 0x6C, 0x61, 0x6E, 0x67, 0x2F, 0x53, 0x79, 0x73, 0x74, 0x65, 0x6D, 0x01, 0x00, 0x09, 0x61, 0x72,
		0x72, 0x61, 0x79, 0x63, 0x6F, 0x70, 0x79, 0x01, 0x00, 0x2A, 0x28, 0x4C, 0x6A, 0x61, 0x76, 0x61, 0x2F, 0x6C, 0x61, 0x6E, 0x67, 0x2F, 0x4F, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3B, 0x49, 0x4C, 0x6A,
		0x61, 0x76, 0x61, 0x2F, 0x6C, 0x61, 0x6E, 0x67, 0x2F, 0x4F, 0x62, 0x6A, 0x65, 0x63, 0x74, 0x3B, 0x49, 0x49, 0x29, 0x56, 0x00, 0x21, 0x00, 0x07, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
		0x00, 0x01, 0x00, 0x08, 0x00, 0x09, 0x00, 0x01, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x11, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x05, 0x2A, 0xB7, 0x00, 0x01, 0xB1, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x89, 0x00, 0x0B, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x89, 0x00, 0x0D, 0x00, 0x0C, 0x00, 0x01, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x2E, 0x00, 0x05, 0x00, 0x03, 0x00, 0x00, 0x00, 0x22, 0x2B, 0xBE, 0x04,
		0x60, 0xBD, 0x00, 0x02, 0x4D, 0x2C, 0x03, 0xBB, 0x00, 0x03, 0x59, 0x04, 0xB7, 0x00, 0x04, 0x53, 0x2B, 0x03, 0x2C, 0x04, 0x2B, 0xBE, 0xB8, 0x00, 0x05, 0x1A, 0x2C, 0xB8, 0x00, 0x06, 0xB0, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x89, 0x00, 0x0E, 0x00, 0x0C, 0x00, 0x01, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x2E, 0x00, 0x05, 0x00, 0x03, 0x00, 0x00, 0x00, 0x22, 0x2B, 0xBE, 0x05, 0x60, 0xBD, 0x00, 0x02,
		0x4D, 0x2C, 0x03, 0xBB, 0x00, 0x03, 0x59, 0x04, 0xB7, 0x00, 0x04, 0x53, 0x2B, 0x03, 0x2C, 0x05, 0x2B, 0xBE, 0xB8, 0x00, 0x05, 0x1A, 0x2C, 0xB8, 0x00, 0x06, 0xB0, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00
	};
	auto Class = Env->DefineClass( "cppjavahook", nullptr, reinterpret_cast<const jbyte*>( ClassData ), sizeof( ClassData ) );
	if ( !Class )
		Class = Env->FindClass( "cppjavahook" );

	if ( !Class )
		throw std::runtime_error( "DefineClass returned NULL" );

	const JNINativeMethod Method = { const_cast<char*>( "a" ), const_cast<char*>( "(S[Ljava/lang/Object;)[Ljava/lang/Object;" ), reinterpret_cast<void*>( Handler ) };
    if ( auto Error = Env->RegisterNatives( Class, &Method, 1 ); Error )
        throw std::runtime_error( "RegisterNatives failed" );

    auto Classes = GetClasses(); // Might end up calling ClassFileLoadHook
	
	if ( !Callbacks->ClassFileLoadHook )
		Callbacks->ClassFileLoadHook = &ClassFileLoadHook;
    if ( auto Error = JVMTIEnv->SetEventCallbacks( Callbacks, sizeof( jvmtiEventCallbacks ) ); Error )
        throw std::runtime_error( "SetEventCallbacks failed" );

    if ( auto Error = JVMTIEnv->RetransformClasses( Classes.size(), Classes.data() ); Error )
        throw std::runtime_error( "RetransformClasses failed" );
}

void ManuallyFixStackMapTable( ClassFile& ClassFile, ClassFile::StackMapTable_attribute& StackMapTable, size_t HookIndex )
{
    if ( HookIndex == 0 )
    {
        StackMapTable.entries[1].locals[2].tag = 7;
        StackMapTable.entries[1].locals[2].cpool_index = ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "java/util/OptionalLong" ) );
        StackMapTable.entries[1].locals[3].tag = 7;
        StackMapTable.entries[1].locals[3].cpool_index = ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "[Ljava/lang/Object;" ) );

        StackMapTable.entries[2].frame_type += 2;
        ClassFile::verification_type_info Info;
    	Info.tag = 7;
        Info.cpool_index = ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "java/util/Map$Entry" ) );
        StackMapTable.entries[2].locals.emplace_back( Info );
        Info.cpool_index = ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "ug" ) );
        StackMapTable.entries[2].locals.emplace_back( Info );

        StackMapTable.entries[4].frame_type -= 2;

        StackMapTable.entries[7].stack[0].offset += 60;
        StackMapTable.entries[7].stack[1].offset += 60;
        StackMapTable.entries[7].locals.erase( StackMapTable.entries[7].locals.begin() );
        //
        StackMapTable.entries[7].locals[0].tag = 7;
        StackMapTable.entries[7].locals[0].cpool_index = ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "cix" ) );
        StackMapTable.entries[7].locals[1].tag = 1;
        StackMapTable.entries[7].locals[2].tag = 7;
        StackMapTable.entries[7].locals[2].cpool_index = ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "java/util/OptionalLong" ) );
        StackMapTable.entries[7].locals[3].tag = 7;
        StackMapTable.entries[7].locals[3].cpool_index = ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "[Ljava/lang/Object;" ) );
        StackMapTable.entries[7].locals[4].tag = 4;
        //

        StackMapTable.entries[8].stack[0].offset += 60;
        StackMapTable.entries[8].stack[1].offset += 60;
        StackMapTable.entries[8].locals.erase( StackMapTable.entries[8].locals.begin() );
        //
        StackMapTable.entries[8].locals[0].tag = 7;
        StackMapTable.entries[8].locals[0].cpool_index = ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "cix" ) );
        StackMapTable.entries[8].locals[1].tag = 1;
        StackMapTable.entries[8].locals[2].tag = 7;
        StackMapTable.entries[8].locals[2].cpool_index = ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "java/util/OptionalLong" ) );
        StackMapTable.entries[8].locals[3].tag = 7;
        StackMapTable.entries[8].locals[3].cpool_index = ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "[Ljava/lang/Object;" ) );
        StackMapTable.entries[8].locals[4].tag = 4;
        StackMapTable.entries[8].locals[5].tag = 7;
        StackMapTable.entries[8].locals[5].cpool_index = ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "gh" ) );
        //

        Info.tag = 0;
        StackMapTable.entries[9].locals.emplace( StackMapTable.entries[9].locals.begin() + 2, Info );
        StackMapTable.entries[9].locals.emplace( StackMapTable.entries[9].locals.begin() + 2, Info );
        StackMapTable.entries[9].locals[5].cpool_index = ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "gh" ) );
        //
        StackMapTable.entries[9].locals[0].tag = 7;
        StackMapTable.entries[9].locals[0].cpool_index = ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "cix" ) );
        StackMapTable.entries[9].locals[1].tag = 1;
        StackMapTable.entries[9].locals[2].tag = 7;
        StackMapTable.entries[9].locals[2].cpool_index = ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "java/util/OptionalLong" ) );
        StackMapTable.entries[9].locals[3].tag = 7;
        StackMapTable.entries[9].locals[3].cpool_index = ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "cix" ) );
        StackMapTable.entries[9].locals[4].tag = 4;
        //
    }
}

void JavaHook::ClassFileLoadHook( jvmtiEnv* jvmti_env, JNIEnv* jni_env, jclass class_being_redefined, jobject loader, const char* name, jobject protection_domain, jint class_data_len,
								  const unsigned char* class_data, jint* new_class_data_len, unsigned char** new_class_data )
{
	if ( !name || s_JavaHook->m_Unhook )
		return;

	std::vector<std::pair<Hook*, size_t>> Hooks;
	for ( size_t i = 0; i < s_JavaHook->m_Hooks.size(); ++i )
	{
		auto& Hook = s_JavaHook->m_Hooks[i];
		if ( Hook.Class == name )
			Hooks.emplace_back( &Hook, i );
	}

	if ( Hooks.empty() )
		return;

	ClassFile ClassFile( class_data );

	for ( const auto& [Hook, HookIndex] : Hooks )
	{
		auto Method = ClassFile.FindMethod( Hook->Name, Hook->Descriptor );
		if ( !Method )
			continue; // throw?

		auto Code = ClassFile.GetCodeAttribute( Method );
		if ( !Code )
			continue; // TO DO: Hook native methods?

		auto CodeSize = Code->code.size();
		
		auto it = Code->code.begin();
		it = Code->PushShort( HookIndex, it );

		auto Parameters = GetParameters( Hook->Descriptor );
		if ( !( Method->access_flags & ClassFile::ACC_STATIC ) )
			Parameters.insert( Parameters.begin(), { Type::Object, name } );

		it = Code->PushInt( ClassFile, Parameters.size(), it );
		it = Code->ANewArray( ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "java/lang/Object" ) ), it );
		
		for ( size_t i = 0; i < Parameters.size(); ++i )
		{
			it = Code->Dup( it ); // Array ref
			it = Code->PushInt( ClassFile, i, it ); // Array index

			if ( Parameters[i].Type == Type::Integer )
			{
				it = Code->ILoad( i, it );
				it = Code->InvokeStatic( ClassFile.FindOrAddMethodRef( ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "java/lang/Integer" ) ),
																	   ClassFile.FindOrAddNameAndType( ClassFile.FindOrAddUTF8( "valueOf" ), ClassFile.FindOrAddUTF8( "(I)Ljava/lang/Integer;" ) ) ), it );
			}
			else if ( Parameters[i].Type == Type::Boolean )
			{
				it = Code->ILoad( i, it );
				it = Code->InvokeStatic( ClassFile.FindOrAddMethodRef( ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "java/lang/Boolean" ) ),
																	   ClassFile.FindOrAddNameAndType( ClassFile.FindOrAddUTF8( "valueOf" ), ClassFile.FindOrAddUTF8( "(Z)Ljava/lang/Boolean;" ) ) ), it );
			}
			else
				it = Code->ALoad( i, it );

			it = Code->AAStore( it );
		}

		auto ReturnType = GetReturnType( Hook->Descriptor );
		if ( ReturnType.Type == Type::Void )
		{
			it = Code->InvokeStatic( ClassFile.FindOrAddMethodRef( ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "cppjavahook" ) ),
																   ClassFile.FindOrAddNameAndType( ClassFile.FindOrAddUTF8( "b" ), ClassFile.FindOrAddUTF8( "(S[Ljava/lang/Object;)[Ljava/lang/Object;" ) ) ), it );
			Hook->IsVoid = true;
		}
		else
			it = Code->InvokeStatic( ClassFile.FindOrAddMethodRef( ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "cppjavahook" ) ),
																   ClassFile.FindOrAddNameAndType( ClassFile.FindOrAddUTF8( "c" ), ClassFile.FindOrAddUTF8( "(S[Ljava/lang/Object;)[Ljava/lang/Object;" ) ) ), it );

		it = Code->AStore( Parameters.size(), it );
		it = Code->ALoad( Parameters.size(), it );
		it = Code->PushInt( ClassFile, 0, it );
		it = Code->AALoad( it );

		it = Code->CheckCast( ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "java/lang/Boolean" ) ), it );

		it = Code->InvokeVirtual( ClassFile.FindOrAddMethodRef( ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "java/lang/Boolean" ) ),
																ClassFile.FindOrAddNameAndType( ClassFile.FindOrAddUTF8( "booleanValue" ), ClassFile.FindOrAddUTF8( "()Z" ) ) ), it );
		auto JumpAt = Code->code.size();

		if ( ReturnType.Type == Type::Void )
			it = Code->Return( it );
		else
		{
			it = Code->ALoad( Parameters.size(), it );
			it = Code->PushInt( ClassFile, 1, it );
			it = Code->AALoad( it );
			
			if ( ReturnType.Type == Type::Object )
			{
				it = Code->CheckCast( ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( ReturnType.Name ) ), it );
				it = Code->AReturn( it );
			}
		}

		auto FrameEnd = Code->code.size() - CodeSize;
		auto JumpSize = Code->code.size() - JumpAt + 3; // 3 = size of ifne
		it = Code->IfNE( JumpSize, Code->code.begin() + FrameEnd - ( Code->code.size() - JumpAt ) ) + JumpSize - 3;
		FrameEnd += 3;

		auto ParametersStart = 1; // Skip the "continue?" Boolean
		if ( ReturnType.Type != Type::Void ) // Skip the return value
			++ParametersStart;

		size_t ParametersIndex = 0;
		if ( !( Method->access_flags & ClassFile::ACC_STATIC ) ) // Skip the "this" variable
			++ParametersIndex;
		
		for ( ; ParametersIndex < Parameters.size(); ++ParametersIndex )
		{
			it = Code->ALoad( Parameters.size(), it );
			it = Code->PushInt( ClassFile, ParametersStart + ParametersIndex, it );
			it = Code->AALoad( it );

			if ( Parameters[ParametersIndex].Type == Type::Object )
			{
				it = Code->CheckCast( ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( Parameters[ParametersIndex].Name ) ), it );
				it = Code->AStore( ParametersIndex, it );
			}
			else if ( Parameters[ParametersIndex].Type == Type::Integer )
			{
				it = Code->CheckCast( ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "java/lang/Integer" ) ), it );
				it = Code->InvokeVirtual( ClassFile.FindOrAddMethodRef( ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "java/lang/Integer" ) ),
																		ClassFile.FindOrAddNameAndType( ClassFile.FindOrAddUTF8( "intValue" ), ClassFile.FindOrAddUTF8( "()I" ) ) ), it );
				it = Code->IStore( ParametersIndex, it );
			}
			else if ( Parameters[ParametersIndex].Type == Type::Boolean )
			{
				it = Code->CheckCast( ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "java/lang/Boolean" ) ), it );
				it = Code->InvokeVirtual( ClassFile.FindOrAddMethodRef( ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "java/lang/Boolean" ) ),
																		ClassFile.FindOrAddNameAndType( ClassFile.FindOrAddUTF8( "booleanValue" ), ClassFile.FindOrAddUTF8( "()Z" ) ) ), it );
				it = Code->IStore( ParametersIndex, it );
			}
		}

        Code->max_stack += Parameters.size();
		Code->max_locals += 1;
		
		CodeSize = Code->code.size() - CodeSize;

		auto& StackMapTable = Code->FindOrCreateStackMapTableOnly( ClassFile );
		if ( !StackMapTable.entries.empty() )
		{
			auto& Frame = StackMapTable.entries.front();
			if ( Frame.frame_type <= 63 )
			{
				auto Offset = CodeSize - FrameEnd + Frame.frame_type - 1;
				if ( Offset > 63 )
				{
					Frame.frame_type = 251; // same_frame_extended
					Frame.offset_delta = Offset;
				}
				else
					Frame.frame_type = Offset;
			}
			else if ( Frame.frame_type >= 64 && Frame.frame_type <= 127 )
			{
				auto Offset = CodeSize - FrameEnd + Frame.frame_type - 65;
				if ( Offset + 64 > 127 )
				{
					Frame.frame_type = 247; // same_locals_1_stack_item_frame_extended
					Frame.offset_delta = Offset;
				}
				else
					Frame.frame_type = Offset + 64;
			}
			else if ( Frame.frame_type >= 247 && Frame.frame_type <= 255 )
				Frame.offset_delta = CodeSize - FrameEnd + Frame.offset_delta - 1;
		}

		ClassFile::stack_map_frame AppendFrame;
		AppendFrame.frame_type = 252;
		AppendFrame.offset_delta = FrameEnd;
		ClassFile::verification_type_info Info;
		Info.tag = 7;
		Info.cpool_index = ClassFile.FindOrAddClass( ClassFile.FindOrAddUTF8( "[Ljava/lang/Object;" ) );
		AppendFrame.locals.emplace_back( Info );

		if ( StackMapTable.entries.empty() )
			StackMapTable.entries.emplace_back( AppendFrame );
		else
			StackMapTable.entries.emplace( StackMapTable.entries.begin(), AppendFrame );

		// That's what's left for me to do so I can release this library.
		// Right now I can't figure a way to automatically do that without analyzing all the bytecodes of a function, so here it is hardcoded for this project's functions.
		// For simple functions just the code above is fine.
        ManuallyFixStackMapTable( ClassFile, StackMapTable, HookIndex );
	}
	
	*new_class_data_len = ClassFile.GetSize();
    if ( auto Error = jvmti_env->Allocate( *new_class_data_len, new_class_data ); Error )
        throw std::runtime_error( "Failed to allocate class data" );
	
	ClassFile.CopyTo( *new_class_data );
}

jobjectArray JavaHook::Handler( JNIEnv* env, jclass obj, jshort MethodID, jobjectArray Arguments )
{
	auto& Hook = s_JavaHook->m_Hooks[MethodID];
	if ( Hook.Handler )
		Hook.Handler( env, HookData( env, Arguments, Hook.IsVoid ) );
	
	return Arguments;
}

std::vector<jclass> JavaHook::GetClasses()
{
	std::vector<const std::string*> ClassNames;
	for ( const auto& Hook : m_Hooks )
	{
		if ( std::find( ClassNames.begin(), ClassNames.end(), &Hook.Class ) == ClassNames.end() )
			ClassNames.emplace_back( &Hook.Class );
	}

	std::vector<jclass> Classes;
	for ( const auto& ClassName : ClassNames )
	{
		auto HookClass = m_Env->FindClass( ClassName->c_str() );
        if ( !HookClass )
            throw std::runtime_error( "Class " + *ClassName + " not found" );

		Classes.emplace_back( HookClass );
	}

	return Classes;
}

std::vector<JavaHook::NamedType> JavaHook::GetParameters( const std::string_view& Descriptor ) // TO DO: More checks
{
	std::vector<NamedType> Parameters;
	for ( size_t i = 1; i < Descriptor.length(); ++i )
	{
		if ( Descriptor[i] == ')' )
			break;

		auto ParameterType = static_cast<Type>( Descriptor[i] );
		std::string ParameterName;
		if ( ParameterType == Type::Object )
		{
			auto NameEnd = Descriptor.find_first_of( ';', i );
			ParameterName = Descriptor.substr( i + 1, NameEnd - i - 1 );
			i = NameEnd;
		}

		Parameters.emplace_back( ParameterType, ParameterName );
	}

	return Parameters;
}

JavaHook::NamedType JavaHook::GetReturnType( const std::string_view& Descriptor ) // TO DO: More checks
{
	auto ParametersEnd = Descriptor.find_last_of( ')' );
	if ( ParametersEnd == std::string_view::npos )
		throw std::runtime_error( "Invalid descriptor" );
	++ParametersEnd;
	
	auto ReturnType = static_cast<Type>( Descriptor[ParametersEnd] );
	
	std::string ReturnName;
	if ( ReturnType == Type::Object )
		ReturnName = Descriptor.substr( ParametersEnd + 1, Descriptor.find_first_of( ';', ParametersEnd ) - ParametersEnd - 1 );

	return { ReturnType, ReturnName };
}

JavaHook::HookData::HookData( JNIEnv* Env, jobjectArray Arguments, bool IsVoid ) :
	m_Env( Env ),
	m_Arguments( Arguments ),
	m_IsVoid( IsVoid )
{ }

void JavaHook::HookData::SetArgument( jsize Index, jobject Value ) const
{
	if ( !m_IsVoid )
		++Index;

	m_Env->SetObjectArrayElement( m_Arguments, Index + 1, Value );
}

void JavaHook::HookData::SetReturn( jobject Return ) const
{
	if ( m_IsVoid )
		throw std::runtime_error( "SetReturn called on void function" );
	
	m_Env->SetObjectArrayElement( m_Arguments, 0, m_Env->CallStaticObjectMethod( GetBooleanClass( m_Env ), GetBooleanValueOfMethod( m_Env ), false ) );
	m_Env->SetObjectArrayElement( m_Arguments, 1, Return );
}

jclass JavaHook::HookData::GetBooleanClass( JNIEnv* Env )
{
	static auto Boolean = Env->FindClass( "java/lang/Boolean" );
	if ( !Boolean )
		throw std::runtime_error( "Failed to find java/lang/Boolean" );

	return Boolean;
}

jmethodID JavaHook::HookData::GetBooleanValueOfMethod( JNIEnv* Env )
{
	static auto valueOf = Env->GetStaticMethodID( GetBooleanClass( Env ), "valueOf", "(Z)Ljava/lang/Boolean;" );
	if ( !valueOf )
		throw std::runtime_error( "Failed to find Boolean.valueOf" );
	
	return valueOf;
}