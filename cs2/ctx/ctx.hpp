#pragma once

namespace valve {
    struct cvar_t;
}

class c_ctx {
private:
	using modules_t = std::unordered_map< sdk::hash_t, sdk::pe_image_t* >;

    using interfaces_t = std::unordered_map< sdk::hash_t, sdk::address_t >;

    struct ent_offset_t {
        std::uint32_t m_offset{};
    };

    using ent_offsets_t = std::unordered_map< sdk::hash_t, ent_offset_t >;

	bool wait_for_all_modules( modules_t& modules ) const;

    void init_imgui( const modules_t& modules ) const;

    void parse_interfaces( sdk::pe_image_t* const image, interfaces_t& interfaces ) const;

    void init_interfaces( const modules_t& modules ) const;

    bool parse_ent_offsets( ent_offsets_t& offsets ) const;

    void init_offsets( const modules_t& modules );

    void init_hooks( const modules_t& modules ) const;

    void init_cvars( );

    struct {
        sdk::address_t m_split_screen_view_controller{},
                       m_split_screen_view_pawn{};

        struct {
            std::uint32_t m_flags{}, m_move_type{};
        } m_base_entity{};

        struct {
            sdk::address_t m_user_cmd_manager{};
        } m_base_player_controller{};

        struct {
            std::uint32_t m_pawn_alive{};
        } m_cs_player_controller{};
    } m_offsets{};

    struct {
        valve::cvar_t* sv_autobunnyhopping{};
    } m_cvars{};
public:
	void init( );

    ALWAYS_INLINE const auto& offsets( ) const;

    ALWAYS_INLINE const auto& cvars( ) const;
};

inline const auto g_ctx = std::make_unique< c_ctx >( );

#include "impl/ctx.inl"