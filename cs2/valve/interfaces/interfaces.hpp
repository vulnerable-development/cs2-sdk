#pragma once

namespace valve {
    class c_engine {
    public:
        VFUNC( bool( __thiscall* )( decltype( this ) ), in_game( ), 38u );
    } inline* g_engine{};

    class c_entity_list {
    public:
        POFFSET_VFUNC( cs_player_controller_t*( __thiscall* )( const int ),
            split_screen_view_controller( const int index ), g_ctx->offsets( ).m_split_screen_view_controller, index
        );

        POFFSET_VFUNC( cs_player_pawn_t*( __thiscall* )( const int ),
            split_screen_view_pawn( const int index ), g_ctx->offsets( ).m_split_screen_view_pawn, index
        );
    } inline* g_entity_list{};

    class c_schema_system {
    public:
        VFUNC( schema_type_scope_t*( __thiscall* )( decltype( this ), const char*, const void* ),
            find_type_scope( const char* name ), 13u, name, 0
        );
    } inline* g_schema_system{};

    struct input_system_t {} inline* g_input_system{};

    struct input_t {
        std::byte       pad0[ 593u ]{};
        bool            m_camera_in_third_person{};
        std::byte       pad1[ 6u ]{};
        sdk::qang_t     m_third_person_angle{};
    } inline* g_input{};

    class c_cvar {
    public:
        ALWAYS_INLINE cvar_t* find_var( const sdk::hash_t name );

        ALWAYS_INLINE std::uint64_t first( );

        ALWAYS_INLINE std::uint64_t next( const std::uint64_t& value );

        VFUNC( cvar_t*( __thiscall* )( decltype( this ), const std::uint64_t ),
            find_by_index( const std::uint64_t index ), 43u, index
        );
    } inline* g_cvar{};
}

#include "impl/interfaces.inl"