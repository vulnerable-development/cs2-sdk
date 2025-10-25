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
        HASH( "inputsystem.dll" )
    };

    interfaces_t interfaces{};
    for ( const auto hash : k_needed_modules )
        parse_interfaces( modules.at( hash ), interfaces );

    if ( interfaces.empty( ) )
        THROW_IF_DBG( "can't find interfaces." );

    valve::g_input_system = interfaces.at( HASH( "InputSystemVersion001" ) ).as< valve::input_system_t* >( );
    valve::g_input = *BYTESEQ( "48 8B 0D ? ? ? ? 4C 8B C6 8B 10 E8" ).search(
        client.m_start, client.m_end
    ).self_rel( ).as< valve::input_t** >( );
}

void c_ctx::init_hooks( const modules_t& modules ) const {
    HOOK_VFUNC( valve::g_input_system, 76u, hooks::is_relative_mouse_mode, hooks::o_is_relative_mouse_mode );

    HOOK_VFUNC( valve::g_input, 19u, hooks::mouse_input_enabled, hooks::o_mouse_input_enabled );
}

void c_ctx::init( ) const {
    modules_t modules{};
    while ( wait_for_all_modules( modules ) )
        std::this_thread::sleep_for( std::chrono::milliseconds{ 200u } );

    if ( MH_Initialize( ) != MH_OK )
        THROW_IF_DBG( "can't initialize minhook." );

    init_imgui( modules );

    init_interfaces( modules );

    init_hooks( modules );

    if ( MH_EnableHook( MH_ALL_HOOKS ) != MH_OK )
        THROW_IF_DBG( "can't enable all hooks." );
}

#undef HOOK
#undef HOOK_VFUNC

#undef THROW_IF_DBG