#pragma once

namespace valve {
    ALWAYS_INLINE cvar_t* c_cvar::find_var( const sdk::hash_t name ) {
        for ( auto it = first( ); it != static_cast< std::uint64_t >( -1u ); it = next( it ) ) {
            const auto var = find_by_index( it );
            if ( !var )
                continue;

            if ( sdk::hash( var->m_name ) == name )
                return var;
        }

        return nullptr;
    }

    ALWAYS_INLINE std::uint64_t c_cvar::first( ) {
        using fn_t = void( __thiscall* )( decltype( this ), std::uint64_t* );

        std::uint64_t ret{};

        ( *sdk::address_t{ this }.as< fn_t** >( ) )[ 12u ]( this, &ret );

        return ret;
    }

    ALWAYS_INLINE std::uint64_t c_cvar::next( const std::uint64_t& value ) {
        using fn_t = void( __thiscall* )( decltype( this ), std::uint64_t*, std::uint64_t );

        std::uint64_t ret{ value };

        ( *sdk::address_t{ this }.as< fn_t** >( ) )[ 13u ]( this, &ret, value );

        return ret;
    }
}