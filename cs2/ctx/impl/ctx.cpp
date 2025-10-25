#include "../../cs2.hpp"

int __stdcall DllMain( _In_ HINSTANCE instance, _In_ DWORD reason, [[maybe_unused]] _In_ LPVOID reserved ) {
    if ( reason != DLL_PROCESS_ATTACH )
        return 0;

    DisableThreadLibraryCalls( instance );

    std::jthread{ [ ] ( ) { g_ctx->init( ); } }.detach( );

    return 1;
}

#ifdef _DEBUG
#define THROW_IF_DBG( exception ) throw std::runtime_error{ exception }
#else
#define THROW_IF_DBG( exception ) return
#endif

#define HOOK( target, hook, original )                                                             \
    if ( MH_CreateHook( sdk::address_t{ target }.as< LPVOID >( ),                                  \
       reinterpret_cast< LPVOID >( &hook ), reinterpret_cast< LPVOID* >( &original ) ) != MH_OK )  \
       THROW_IF_DBG( "can't hook " #hook "." )                                                     \

#define HOOK_VFUNC( vft, index, hook, original )                                                   \
    if ( MH_CreateHook( ( *sdk::address_t{ vft }.as< LPVOID** >( ) )[ index ],                     \
        reinterpret_cast< LPVOID >( &hook ), reinterpret_cast< LPVOID* >( &original ) ) != MH_OK ) \
        THROW_IF_DBG( "can't hook " #hook "." )                                                    \

struct code_section_t {
    ALWAYS_INLINE constexpr code_section_t( ) = default;

    ALWAYS_INLINE code_section_t( const sdk::pe_image_t* const image ) {
        if ( image->m_dos_hdr.e_magic != sdk::k_dos_hdr_magic )
            THROW_IF_DBG( "invalid dos hdr." );

        const auto nt_hdrs = image->nt_hdrs( );
        if ( nt_hdrs->m_sig != sdk::k_nt_hdrs_magic )
            THROW_IF_DBG( "invalid nt hdrs." );

        m_start = image;
        m_start.self_offset( nt_hdrs->m_opt_hdr.m_code_base );

        m_end = m_start.offset( nt_hdrs->m_opt_hdr.m_code_size );
    }

    sdk::address_t m_start{}, m_end{};
};

bool c_ctx::wait_for_all_modules( modules_t& modules ) const {
    sdk::peb( )->for_each_ldr_data_table_entry( [ & ] ( sdk::ldr_data_table_entry_t* const entry ) {
        modules.insert_or_assign(
            sdk::hash( entry->m_base_dll_name.m_buffer, entry->m_base_dll_name.m_length / sizeof( wchar_t ) ),
            entry->m_dll_base.as< sdk::pe_image_t* >( )
        );

        return false;
    }, sdk::e_ldr_data_table::in_load_order );

    return !modules.contains( HASH( "navsystem.dll" ) );
}

void c_ctx::init_imgui( const modules_t& modules ) const {
    const code_section_t inputsystem{ modules.at( HASH( "inputsystem.dll" ) ) };
    const code_section_t gameoverlayrenderer{ modules.at( HASH( "gameoverlayrenderer64.dll" ) ) };

    HOOK( BYTESEQ( "48 83 EC ? 4C 89 4C 24" ).search(
        inputsystem.m_start, inputsystem.m_end
    ), hooks::wnd_proc, hooks::o_wnd_proc );

    HOOK( BYTESEQ( "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC ? 41 8B E8" ).search(
        gameoverlayrenderer.m_start, gameoverlayrenderer.m_end
    ), hooks::dx11_present, hooks::o_dx11_present );

    HOOK( BYTESEQ( "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 54 41 56 41 57 48 83 EC ? 44 8B E2" ).search(
        gameoverlayrenderer.m_start, gameoverlayrenderer.m_end
    ), hooks::dx11_resize_buffers, hooks::o_dx11_resize_buffers );
}

void c_ctx::parse_interfaces( sdk::pe_image_t* const image, interfaces_t& interfaces ) const {
    sdk::address_t list{};

    image->for_each_export( image, [ & ] ( const char* name, const sdk::address_t addr ) {
        if ( sdk::hash( name ) != HASH( "CreateInterface" ) )
            return false;

        list = addr;

        return true;
    } );

    if ( !list )
        THROW_IF_DBG( "can't find CreateInterface export." );

    struct interface_entry_t {
        using create_t = std::add_pointer_t< std::uintptr_t( ) >;

        create_t           m_create_fn{};
        const char*        m_name{};
        interface_entry_t* m_next{};
    };

    for ( auto entry = *list.self_rel( ).as< interface_entry_t** >( ); entry; entry = entry->m_next )
        if ( entry->m_name )
            interfaces.insert_or_assign( sdk::hash( entry->m_name ), entry->m_create_fn( ) );
}

void c_ctx::init_interfaces( const modules_t& modules ) const {
    const code_section_t client{ modules.at( HASH( "client.dll" ) ) };

    constexpr sdk::hash_t k_needed_modules[ ] = {
        HASH( "engine2.dll" ),
        HASH( "schemasystem.dll" ),
        HASH( "inputsystem.dll" ),
        HASH( "tier0.dll" )
    };

    interfaces_t interfaces{};
    for ( const auto hash : k_needed_modules )
        parse_interfaces( modules.at( hash ), interfaces );

    if ( interfaces.empty( ) )
        THROW_IF_DBG( "can't find interfaces." );

    valve::g_engine = interfaces.at( HASH( "Source2EngineToClient001" ) ).as< valve::c_engine* >( );

    valve::g_entity_list = *BYTESEQ( "48 89 35 ? ? ? ? 48 85 F6" ).search(
        client.m_start, client.m_end
    ).self_rel( ).as< valve::c_entity_list** >( );

    valve::g_schema_system = interfaces.at( HASH( "SchemaSystem_001" ) ).as< valve::c_schema_system* >( );

    valve::g_input_system = interfaces.at( HASH( "InputSystemVersion001" ) ).as< valve::input_system_t* >( );
    valve::g_input = *BYTESEQ( "48 8B 0D ? ? ? ? 4C 8B C6 8B 10 E8" ).search(
        client.m_start, client.m_end
    ).self_rel( ).as< valve::input_t** >( );

    valve::g_cvar = interfaces.at( HASH( "VEngineCvar007" ) ).as< valve::c_cvar* >( );
}

bool c_ctx::parse_ent_offsets( ent_offsets_t& offsets ) const {
    constexpr const char* k_needed_modules[ ] = {
        "client.dll",
        "engine2.dll",
        "schemasystem.dll"
    };

    offsets.reserve( 41000u );

    std::string concated{};

    concated.reserve( 128u );

    for ( const auto modules : k_needed_modules ) {
        const auto type_scope = valve::g_schema_system->find_type_scope( modules );
        if ( !type_scope )
            continue;

        const auto elements_count = type_scope->m_bindings.count( );
        const auto new_elements = std::make_unique_for_overwrite< std::uintptr_t[ ] >( elements_count );
        const auto elements = type_scope->m_bindings.elements( elements_count, new_elements.get( ) );

        for ( int i{}; i < elements; ++i ) {
            const auto element = new_elements[ i ];

            const auto binding = type_scope->m_bindings[ element ];
            if ( !binding )
                continue;

            valve::schema_info_data_t* class_info{};
            type_scope->find( &class_info, binding->m_name );

            if ( !class_info
                 || class_info->m_fields_count <= 0 )
                continue;

            for ( std::uint8_t j{}; j < class_info->m_fields_count; ++j ) {
                const auto& field = class_info->m_fields[ j ];
                if ( !field.m_name )
                    continue;

                concated.assign( binding->m_name );
                concated.append( "->" );
                concated.append( field.m_name );

                offsets.insert_or_assign(
                    sdk::hash( concated.data( ), concated.size( ) ),
                    ent_offset_t{ field.m_offset }
                );
            }
        }
    }

    return !offsets.empty( );
}

void c_ctx::init_offsets( const modules_t& modules ) {
    const code_section_t client{ modules.at( HASH( "client.dll" ) ) };

    m_offsets.m_split_screen_view_controller = BYTESEQ( "48 83 EC ? 83 F9 ? 75 ? 48 8B 0D ? ? ? ? 48 8D 54 24 ? 48 8B 01 FF 90 ? ? ? ? 8B 08 48 63 C1 48 8D 0D ? ? ? ? 48 8B 04 C1 48 83 C4 ? C3 CC CC CC CC CC CC CC CC CC CC CC CC CC 48 83 EC ? 83 F9" ).search(
        client.m_start, client.m_end
    );

    m_offsets.m_split_screen_view_pawn = BYTESEQ( "48 83 EC ? 83 F9 ? 75 ? 48 8B 0D ? ? ? ? 48 8D 54 24 ? 48 8B 01 FF 90 ? ? ? ? 8B 08 48 63 C1 4C 8D 05" ).search(
        client.m_start, client.m_end
    );

    ent_offsets_t offsets{};
    if ( !parse_ent_offsets( offsets ) )
        THROW_IF_DBG( "can't find ent offsets." );

    m_offsets.m_base_entity.m_flags = offsets.at( HASH( "C_BaseEntity->m_fFlags" ) ).m_offset;
    m_offsets.m_base_entity.m_move_type = offsets.at( HASH( "C_BaseEntity->m_nActualMoveType" ) ).m_offset;

    m_offsets.m_base_player_controller.m_user_cmd_manager = BYTESEQ( "41 56 41 57 48 83 EC ? 48 8D 54 24" ).search(
        client.m_start, client.m_end
    );

    m_offsets.m_cs_player_controller.m_pawn_alive = offsets.at( HASH( "CCSPlayerController->m_bPawnIsAlive" ) ).m_offset;
}

void c_ctx::init_cvars( ) {
    m_cvars.sv_autobunnyhopping = valve::g_cvar->find_var( HASH( "sv_autobunnyhopping" ) );
}

void c_ctx::init_hooks( const modules_t& modules ) const {
    HOOK_VFUNC( valve::g_input_system, 76u, hooks::is_relative_mouse_mode, hooks::o_is_relative_mouse_mode );

    HOOK_VFUNC( valve::g_input, 19u, hooks::mouse_input_enabled, hooks::o_mouse_input_enabled );

    HOOK_VFUNC( valve::g_input, 5u, hooks::create_move, hooks::o_create_move );
}

void c_ctx::init( ) {
    modules_t modules{};
    while ( wait_for_all_modules( modules ) )
        std::this_thread::sleep_for( std::chrono::milliseconds{ 200u } );

    if ( MH_Initialize( ) != MH_OK )
        THROW_IF_DBG( "can't initialize minhook." );

    init_imgui( modules );

    init_interfaces( modules );

    init_offsets( modules );

    init_cvars( );

    init_hooks( modules );

    if ( MH_EnableHook( MH_ALL_HOOKS ) != MH_OK )
        THROW_IF_DBG( "can't enable all hooks." );
}

#undef HOOK
#undef HOOK_VFUNC

#undef THROW_IF_DBG