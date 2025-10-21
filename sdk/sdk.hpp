#ifndef CS2_SDK_SDK_HPP
#define CS2_SDK_SDK_HPP

#define ALWAYS_INLINE [[clang::always_inline]] inline

// clang-format off
#define ENUM_UNDERLYING_OPERATOR( enum_t )                                                                         \
    ALWAYS_INLINE constexpr auto operator-( const enum_t value ) {                                                 \
        return static_cast< std::underlying_type_t< enum_t > >( value );                                           \
    }                                                                                                              \

#define ENUM_BIT_OPERATOR( enum_t, op, ret_underlying )                                                            \
    template < typename _value_t >                                                                                 \
        requires std::_Is_any_of_v< _value_t, enum_t, std::underlying_type_t< enum_t > >                           \
    ALWAYS_INLINE constexpr auto operator op( const enum_t lhs, const _value_t rhs ) {                             \
        using underlying_t = std::underlying_type_t< enum_t >;                                                     \
                                                                                                                   \
        using ret_t = std::conditional_t< ret_underlying, underlying_t, enum_t >;                                  \
                                                                                                                   \
        return static_cast< ret_t >( static_cast< underlying_t >( lhs ) op static_cast< underlying_t >( rhs ) );   \
    }                                                                                                              \
                                                                                                                   \
    ALWAYS_INLINE constexpr auto operator op( const std::underlying_type_t< enum_t > lhs, const enum_t rhs ) {     \
        using underlying_t = std::underlying_type_t< enum_t >;                                                     \
                                                                                                                   \
        using ret_t = std::conditional_t< ret_underlying, underlying_t, enum_t >;                                  \
                                                                                                                   \
        return static_cast< ret_t >( static_cast< underlying_t >( lhs ) op static_cast< underlying_t >( rhs ) );   \
    }                                                                                                              \
                                                                                                                   \
    template < typename _value_t >                                                                                 \
        requires std::_Is_any_of_v< _value_t, enum_t, std::underlying_type_t< enum_t > >                           \
    ALWAYS_INLINE auto& operator op##=( enum_t & lhs, const _value_t rhs ) {                                       \
        using underlying_t = std::underlying_type_t< enum_t >;                                                     \
                                                                                                                   \
        using ret_t = std::conditional_t< ret_underlying, underlying_t, enum_t >;                                  \
                                                                                                                   \
        return reinterpret_cast< ret_t& >(                                                                         \
            reinterpret_cast< underlying_t& >( lhs ) op##= static_cast< underlying_t >( rhs )                      \
        );                                                                                                         \
    }                                                                                                              \

#define ENUM_BIT_OPERATORS( enum_t, ret_underlying )                                                               \
    ENUM_BIT_OPERATOR( enum_t, |, ret_underlying )                                                                 \
                                                                                                                   \
    ENUM_BIT_OPERATOR( enum_t, &, ret_underlying )                                                                 \
                                                                                                                   \
    ENUM_BIT_OPERATOR( enum_t, ^, ret_underlying )                                                                 \
                                                                                                                   \
    ALWAYS_INLINE constexpr auto operator~( const enum_t value ) {                                                 \
        using underlying_t = std::underlying_type_t< enum_t >;                                                     \
                                                                                                                   \
        using ret_t = std::conditional_t< ret_underlying, underlying_t, enum_t >;                                  \
                                                                                                                   \
        return static_cast< ret_t >( ~static_cast< underlying_t >( value ) );                                      \
    }                                                                                                              \
// clang-format on

#include <d3d11.h>
#include <intrin.h>
#include <windows.h>

#include <algorithm>
#include <array>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <optional>
#include <thread>
#include <type_traits>
#include <vector>

#include "mem/mem.hpp"
#include "util/util.hpp"

#endif