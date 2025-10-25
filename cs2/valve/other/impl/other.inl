#pragma once

#include "../other.hpp"

namespace valve {
    ALWAYS_INLINE user_cmd_t* user_cmd_manager_t::get_user_cmd( ) {
        return &m_cmds[ m_sequence_number % k_mp_backup ];
    }

    ALWAYS_INLINE user_cmd_t* user_cmd_manager_t::get_user_cmd( const int sequence_number ) {
        return &m_cmds[ sequence_number % k_mp_backup ];
    }
}