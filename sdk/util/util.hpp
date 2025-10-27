#pragma once

#include "detail/detail.hpp"

#define BYTESEQ( value )                                                                                              \
    [ & ] ( ) {                                                                                                       \
        constexpr auto k_size = sdk::detail::byte_seq_t< sizeof( value ) >::measure( value );                         \
                                                                                                                      \
        return sdk::detail::byte_seq_t< k_size >{ [ & ]( ) { return value; }, std::make_index_sequence< k_size >{} }; \
    } ( )                                                                                                             \

#define HASH( str ) [ ] ( ) { constexpr auto k_hash = sdk::hash( str ); return k_hash; } ( )

namespace sdk {
    using hash_t = std::size_t;

    template < typename _type > inline constexpr auto is_char_v = std::is_same_v< _type, char >
        || std::is_same_v< _type, char16_t > || std::is_same_v< _type, char32_t > || std::is_same_v< _type, wchar_t >;

    ALWAYS_INLINE std::string to_multi_byte( const std::wstring_view wstr );

    ALWAYS_INLINE std::wstring to_wide_char( const std::string_view str );

    template < typename _char_t >
        requires is_char_v< _char_t >
    ALWAYS_INLINE constexpr hash_t hash( const _char_t* const str, const std::size_t length );

    template < typename _char_t >
        requires is_char_v< _char_t >
    ALWAYS_INLINE constexpr hash_t hash( const _char_t* const str );

    ALWAYS_INLINE void attach_console( );
}

#include "impl/util.inl"