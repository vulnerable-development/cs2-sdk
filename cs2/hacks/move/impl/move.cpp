#include "../../../cs2.hpp"

namespace hacks {
    void c_move::run( valve::user_cmd_t* const cmd ) const {
        const auto move_type = g_local_player->pawn( )->move_type( );
        if ( move_type == valve::e_move_type::noclip
            || move_type == valve::e_move_type::ladder )
            return;

        const auto& cfg = g_menu->main( ).m_move;
        const auto& cvars = g_ctx->cvars( );

        if ( cfg.m_bhop.get( )
            && !( cvars.sv_autobunnyhopping->as< bool >( ) ) ) {
            static bool should_fake_jump{};

            if ( should_fake_jump ) {
                cmd->m_buttons.m_pressed |= valve::e_buttons::in_jump;
                should_fake_jump = false;
            }
            else if ( cmd->m_buttons.m_pressed & valve::e_buttons::in_jump ) {
                if ( g_local_player->pawn( )->flags( ) & valve::e_ent_flags::on_ground )
                    cmd->m_buttons.m_pressed &= ~valve::e_buttons::in_jump;
                else
                    should_fake_jump = true;
            }
        }
    }
}