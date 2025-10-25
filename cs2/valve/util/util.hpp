#ifndef CS2_SDK_UTIL_HPP
#define CS2_SDK_UTIL_HPP

namespace valve {
    struct utl_mem_t {
        ALWAYS_INLINE constexpr utl_mem_t( ) = default;

        ALWAYS_INLINE int blocks_count( ) const;

        std::byte pad0[ 12u ]{};
        int       m_blocks_count{};
        std::byte pad1[ 48u ]{};
    };

    template < typename _value_t, int _bucket_count, typename _ret_t = std::uintptr_t >
    struct utl_hash_t {
        ALWAYS_INLINE constexpr utl_hash_t( ) = default;

        ALWAYS_INLINE int count( ) const;

        ALWAYS_INLINE int elements( const int count, std::uintptr_t* value ) const;

        ALWAYS_INLINE _value_t& operator []( const std::uintptr_t i );

        ALWAYS_INLINE const _value_t& operator []( const std::uintptr_t i ) const;

        template < typename _index_t >
        struct hash_data_internal_t {
        public:
            std::byte                         pad0[ 8u ]{};
            hash_data_internal_t< _index_t >* m_next{};
            _index_t						  m_data{};
        };

        struct hash_bucket_t {
            std::byte                         pad0[ 16u ]{};
            hash_data_internal_t< _value_t >* m_first_uncommitted{};
        };

        utl_mem_t     m_mem{};
        std::byte     pad0[ 64u ]{};
        hash_bucket_t m_buckets[ _bucket_count ]{};
        std::byte     pad1{};
    };

    template < typename _value_t >
    struct utl_vec_t {
    private:
        int       m_size{};
        _value_t* m_elements{};
    public:
        ALWAYS_INLINE constexpr utl_vec_t( ) = default;

        ALWAYS_INLINE _value_t& at( const int i );

        ALWAYS_INLINE const _value_t& at( const int i ) const;

        ALWAYS_INLINE int size( ) const;
    };
}

#include "impl/util.inl"

#endif