#pragma once

namespace valve {
    struct base_entity_t {
        OFFSET( e_ent_flags, flags( ), g_ctx->offsets( ).m_base_entity.m_flags );
        OFFSET( e_move_type, move_type( ), g_ctx->offsets( ).m_base_entity.m_move_type );
    };

    struct base_model_entity_t : public base_entity_t {};

    struct base_player_controller_t : public base_model_entity_t {
        OFFSET_VFUNC( user_cmd_manager_t*( __thiscall* )( decltype( this ) ), user_cmd_manager( ), g_ctx->offsets( ).m_base_player_controller.m_user_cmd_manager );
    };

    struct base_player_pawn_t : public base_model_entity_t {};

    struct cs_player_controller_t : public base_player_controller_t {
        OFFSET( bool, alive( ), g_ctx->offsets( ).m_cs_player_controller.m_pawn_alive );
    };

    struct cs_player_pawn_base_t : public base_player_pawn_t {};

    struct cs_player_pawn_t : public cs_player_pawn_base_t {};
}