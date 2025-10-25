#pragma once

#include "../util.hpp"

namespace valve {
    ALWAYS_INLINE int utl_mem_t::blocks_count( ) const { return m_blocks_count; }

    template < typename _value_t, int _bucket_count, typename _ret_t >
    ALWAYS_INLINE int utl_hash_t< _value_t, _bucket_count, _ret_t >::count( ) const { return m_mem.blocks_count( ); }

    template < typename _value_t, int _bucket_count, typename _ret_t  >
    ALWAYS_INLINE int utl_hash_t< _value_t, _bucket_count, _ret_t >::elements( const int count, std::uintptr_t* value ) const {
        int ret{};

        for ( int i{}; i < _bucket_count; ++i ) {
            const auto& bucket = m_buckets[ i ];

            for ( auto it = bucket.m_first_uncommitted; it; it = it->m_next ) {
                value[ ret++ ] = reinterpret_cast< std::uintptr_t >( it );

                if ( ret >= count )
                    return ret;
            }
        }

        return ret;
    }

    template < typename _value_t, int _bucket_count, typename _ret_t >
    ALWAYS_INLINE _value_t& utl_hash_t< _value_t, _bucket_count, _ret_t >::operator []( const std::uintptr_t i ) { return reinterpret_cast< hash_data_internal_t< _value_t >* >( i )->m_data; }

    template < typename _value_t, int _bucket_count, typename _ret_t >
    ALWAYS_INLINE const _value_t& utl_hash_t< _value_t, _bucket_count, _ret_t >::operator []( const std::uintptr_t i ) const { return reinterpret_cast< hash_data_internal_t< _value_t >* >( i )->m_data; }

    template < typename _value_t >
    ALWAYS_INLINE _value_t& utl_vec_t< _value_t >::at( const int i ) { return m_elements[ i ]; }

    template < typename _value_t >
    ALWAYS_INLINE const _value_t& utl_vec_t< _value_t >::at( const int i ) const { return m_elements[ i ]; }

    template < typename _value_t >
    ALWAYS_INLINE int utl_vec_t< _value_t >::size( ) const { return m_size; }
}