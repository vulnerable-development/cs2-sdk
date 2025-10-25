#pragma once

#include "../local_player.hpp"

ALWAYS_INLINE valve::cs_player_controller_t* c_local_player::controller( ) const { return m_local_player_controller; }

ALWAYS_INLINE valve::cs_player_pawn_t* c_local_player::pawn( ) const { return m_local_player_pawn; }