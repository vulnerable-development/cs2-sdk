#pragma once

namespace sdk {
    template < typename _ret_t >
        requires std::is_floating_point_v< _ret_t >
    inline constexpr auto k_pi = static_cast< _ret_t >( 3.141592653589793 );

    template < typename _ret_t >
        requires std::is_floating_point_v< _ret_t >
    inline constexpr auto k_pi2 = static_cast< _ret_t >( k_pi< double > * 2.0 );

    template < typename _ret_t >
        requires std::is_floating_point_v< _ret_t >
    inline constexpr auto k_rad_pi = static_cast< _ret_t >( 180.0 / k_pi< double > );

    template < typename _ret_t >
        requires std::is_floating_point_v< _ret_t >
    inline constexpr auto k_deg_pi = static_cast< _ret_t >( k_pi< double > / 180.0 );

    template < typename _value_t >
        requires std::is_arithmetic_v< _value_t >
    ALWAYS_INLINE constexpr auto to_deg( const _value_t rad );

    template < typename _value_t >
        requires std::is_arithmetic_v< _value_t >
    ALWAYS_INLINE constexpr auto to_rad( const _value_t deg );

    template < typename _value_t >
        requires std::is_floating_point_v< _value_t >
    ALWAYS_INLINE constexpr _value_t normalize_angle( const _value_t angle );
}

#include "detail/detail.hpp"

namespace sdk {
    using vec2_t = detail::base_vec_t< float, 2u >;

    using vec3_t = detail::base_vec_t< float, 3u >;

    using vec4_t = detail::base_vec_t< float, 4u >;

    using qang_t = detail::base_qang_t< float >;

    using mat3x4_t = detail::base_mat_t< float, 3u, 4u >;

    using argb_t = detail::base_argb_t<>;

    using ahsv_t = detail::base_ahsv_t<>;
}