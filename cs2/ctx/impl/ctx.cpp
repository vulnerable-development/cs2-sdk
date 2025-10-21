#include "../../cs2.hpp"

int __stdcall DllMain( _In_ HINSTANCE instance, _In_ DWORD reason, [[maybe_unused]] _In_ LPVOID reserved ) {
    if ( reason != DLL_PROCESS_ATTACH )
        return 0;

    DisableThreadLibraryCalls( instance );

    std::jthread{ [ ] ( ) { g_ctx->init( ); } }.detach( );

    return 1;
}

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

void c_ctx::init( ) const {
    modules_t modules{};
    while ( wait_for_all_modules( modules ) )
        std::this_thread::sleep_for( std::chrono::milliseconds{ 200u } );
}