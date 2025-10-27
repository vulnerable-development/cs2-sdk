#include "../../cs2.hpp"

void c_menu::run( ) {
    if ( m_main.m_hidden )
        return;

    ImGui::Begin( PROJECT_NAME );

    if ( ImGui::CollapsingHeader( "move" ) ) {
        ImGui::Checkbox( "bhop", &m_main.m_move.m_bhop );
    }

    if ( ImGui::CollapsingHeader( "cfg" ) ) {
        if ( ImGui::Button( "load" ) )
            sdk::g_cfg->load( "example" );

        if ( ImGui::Button( "save" ) )
            sdk::g_cfg->save( "example" );
    }

    ImGui::End( );
}