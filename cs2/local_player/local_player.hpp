#pragma once

class c_local_player {
private:
    valve::cs_player_controller_t* m_local_player_controller{};
    valve::cs_player_pawn_t*       m_local_player_pawn{};
public:
    void on_create_move( );

    ALWAYS_INLINE valve::cs_player_controller_t* controller( ) const;

    ALWAYS_INLINE valve::cs_player_pawn_t* pawn( ) const;
};

inline const auto g_local_player = std::make_unique< c_local_player >( );

#include "impl/local_player.inl"