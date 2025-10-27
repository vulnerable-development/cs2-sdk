#include "../../cs2.hpp"

void c_local_player::on_create_move( ) {
    m_local_player_controller = valve::g_entity_list->split_screen_view_controller( 0 );
    m_local_player_pawn = valve::g_entity_list->split_screen_view_pawn( 0 );

    if ( !m_local_player_controller
        || !m_local_player_controller->alive( )
        || !m_local_player_pawn )
        return;

    const auto cmd_manager = m_local_player_controller->user_cmd_manager( );
    const auto cmd = cmd_manager->get_user_cmd( );

    if ( !cmd_manager
        || !cmd )
        return;

    cmd->pb.mutable_base( )->clear_subtick_moves( );

    hacks::g_move->run( cmd );

    {
        CBaseUserCmdPB serialized_cmd{};

        serialized_cmd.mutable_buttons_pb( )->set_buttonstate1( -cmd->m_buttons.m_pressed );
        serialized_cmd.mutable_buttons_pb( )->set_buttonstate2( -cmd->m_buttons.m_changed );
        serialized_cmd.mutable_buttons_pb( )->set_buttonstate3( -cmd->m_buttons.m_scroll );

        serialized_cmd.mutable_viewangles( )->set_x( cmd->pb.base( ).viewangles( ).x( ) );
        serialized_cmd.mutable_viewangles( )->set_y( cmd->pb.base( ).viewangles( ).y( ) );
        serialized_cmd.mutable_viewangles( )->set_z( cmd->pb.base( ).viewangles( ).z( ) );

        serialized_cmd.SerializeToString( cmd->pb.mutable_base( )->mutable_move_crc( ) );
    }
}