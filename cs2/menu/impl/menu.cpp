#include "../../cs2.hpp"

void c_menu::run( ) const {
    if ( m_main.m_hidden )
        return;

    ImGui::Begin( "cs2 sdk" );

    if ( ImGui::CollapsingHeader( "cfg" ) ) {
        if ( ImGui::Button( "load" ) )
            sdk::g_cfg->load( "example" );

        if ( ImGui::Button( "save" ) )
            sdk::g_cfg->save( "example" );
    }

    ImGui::End( );
}