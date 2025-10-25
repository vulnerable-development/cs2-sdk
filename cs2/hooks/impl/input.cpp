#include "../../cs2.hpp"

namespace hooks {
    void __fastcall create_move( valve::input_t* const rcx, const int slot, const bool active ) {
        o_create_move( rcx, slot, active );

        if ( !valve::g_engine->in_game( ) )
            return;

        g_local_player->on_create_move( );
    }
}