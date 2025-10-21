#ifndef CS2_SDK_UTIL_INL
#define CS2_SDK_UTIL_INL

namespace sdk {
	ALWAYS_INLINE std::string to_multi_byte( const std::wstring_view wstr ) {
		if ( wstr.empty( ) )
			return {};

		const auto length = WideCharToMultiByte( CP_UTF8, 0, wstr.data( ), wstr.size( ), 0, 0, 0, 0 );
		if ( length <= 0 )
			return {};

		std::string str{};

		str.resize( length );

		if ( WideCharToMultiByte( CP_UTF8, 0, wstr.data( ), wstr.size( ), str.data( ), length, 0, 0 ) <= 0 )
			return {};

		return str;
	}

	ALWAYS_INLINE std::wstring to_wide_char( const std::string_view str ) {
		if ( str.empty( ) )
			return {};

		const auto length = MultiByteToWideChar( CP_UTF8, 0, str.data( ), str.size( ), 0, 0 );
		if ( length <= 0 )
			return {};

		std::wstring wstr{};

		wstr.resize( length );

		if ( MultiByteToWideChar( CP_UTF8, 0, str.data( ), str.size( ), wstr.data( ), length ) <= 0 )
			return {};

		return wstr;
	}

	template < typename _char_t >
		requires is_char_v< _char_t >
	ALWAYS_INLINE constexpr hash_t hash( const _char_t* const str, const std::size_t length ) {
		constexpr auto k_basis = 0xcbf29ce484222325u;
		constexpr auto k_prime = 0x100000001b3u;

		auto hash = k_basis;

		for ( std::size_t i{}; i < length; ++i )
			hash = ( hash ^ str[ i ] ) * k_prime;

		return hash;
	}

	template < typename _char_t >
		requires is_char_v< _char_t >
	ALWAYS_INLINE constexpr hash_t hash( const _char_t* const str ) {
		std::size_t length{};
		while ( str[ ++length ] != '\0' )
			;

		return hash( str, length );
	}
}

#endif