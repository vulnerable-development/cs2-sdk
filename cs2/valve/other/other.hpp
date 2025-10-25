#pragma once

namespace valve {
    inline constexpr auto k_mp_backup = 150u;

#pragma region input
    enum struct e_buttons : std::uint64_t {
        in_attack      = 1u << 0u,
        in_jump        = 1u << 1u,
        in_duck        = 1u << 2u,
        in_fwd         = 1u << 3u,
        in_back        = 1u << 4u,
        in_use         = 1u << 5u,
        in_left        = 1u << 7u,
        in_right       = 1u << 8u,
        in_move_left   = 1u << 9u,
        in_move_right  = 1u << 10u,
        in_move        = in_fwd | in_back | in_move_right | in_move_left,
        in_attack2     = 1u << 11u,
        in_reload      = 1u << 13u,
        in_sprint      = 1u << 16u,
        in_score       = 1ull << 33u,
        in_zoom        = 1ull << 34u,
        in_look_weapon = 1ull << 35u
    };
    ENUM_BIT_OPERATORS( e_buttons, true );
    ENUM_UNDERLYING_OPERATOR( e_buttons );

    struct button_state_t {
        std::byte pad0[ 8u ]{};
        e_buttons m_pressed{},
                  m_changed{},
                  m_scroll{};
    };

    struct user_cmd_t {
        std::byte           pad0[ 8u ]{};
        int                 m_sequence_number{};
        CSGOUserCmdPB       pb{};
        button_state_t      m_buttons{};
        std::byte           pad1[ 25u ]{};
    };

    struct user_cmd_manager_t {
        ALWAYS_INLINE user_cmd_t* get_user_cmd( );

        ALWAYS_INLINE user_cmd_t* get_user_cmd( const int sequence_number );

        user_cmd_t m_cmds[ k_mp_backup ],
                   m_previous_cmd{};
        int        m_sequence_number{};
        std::byte  pad0[ 2092u ]{};
    };
#pragma endregion

#pragma region schema
    struct schema_binding_t {
        std::byte   pad0[ 8u ]{};
        const char* m_name{};
        std::byte   pad1[ 48u ]{};
    };

    struct schema_field_data_t {
        const char*   m_name{};
        std::byte     pad0[ 8u ]{};
        std::uint32_t m_offset{};
        std::byte     pad1[ 12u ]{};
    };

    struct schema_info_data_t {
        std::byte            pad0[ 8u ]{};
        const char*          m_name{};
        std::byte            pad2[ 12u ]{};
        std::uint8_t         m_fields_count{};
        std::byte            pad3[ 8u ]{};
        schema_field_data_t* m_fields{};
    };

    struct schema_type_scope_t {
        VFUNC( void( __thiscall* )( decltype( this ), schema_info_data_t**, const char* ),
            find( schema_info_data_t** ret, const char* name ), 2u, ret, name
        );

        std::byte                                            pad0[ 1280u ]{};
        utl_hash_t< schema_binding_t*, 256u, std::uint32_t > m_bindings{};
        std::byte                                            pad1[ 6u ]{};
    };
#pragma endregion

    struct cvar_t {
        template < typename _value_t >
        OFFSET( _value_t, as( ), 0x50u );

        const char*   m_name{};
        cvar_t*       m_next{};
        std::byte     pad0[ 16u ]{};
        const char*   m_description{};
        std::uint32_t m_type{},
                      m_registered{},
                      m_flags{};
        std::byte     pad1[ 44u ]{};
    };

#pragma region player
    enum struct e_ent_flags : std::uint32_t {
        on_ground             = 1u << 0u,
        ducking               = 1u << 1u,
        water_jump            = 1u << 2u,
        on_train              = 1u << 3u,
        in_rain               = 1u << 4u,
        frozen                = 1u << 5u,
        at_controls           = 1u << 6u,
        client                = 1u << 7u,
        fake_client           = 1u << 8u,
        in_water              = 1u << 9u,
        fly                   = 1u << 10u,
        swim                  = 1u << 11u,
        conveyor              = 1u << 12u,
        npc                   = 1u << 13u,
        godmode               = 1u << 14u,
        no_target             = 1u << 15u,
        aim_target            = 1u << 16u,
        partial_ground        = 1u << 17u,
        static_prop           = 1u << 18u,
        graphed               = 1u << 19u,
        grenade               = 1u << 20u,
        step_movement         = 1u << 21u,
        dont_touch            = 1u << 22u,
        base_velocity         = 1u << 23u,
        world_brush           = 1u << 24u,
        object                = 1u << 25u,
        kill_me               = 1u << 26u,
        on_fire               = 1u << 27u,
        dissolving            = 1u << 28u,
        ragdoll               = 1u << 29u,
        unblockable_by_player = 1u << 30u
    };
    ENUM_BIT_OPERATORS( e_ent_flags, true );

    enum struct e_move_type : std::uint8_t {
        none,
        obsolete,
        walk,
        fly,
        fly_gravity,
        vphysics,
        push,
        noclip,
        observer,
        ladder,
        custom,
        last
    };
#pragma endregion
}

#include "impl/other.inl"