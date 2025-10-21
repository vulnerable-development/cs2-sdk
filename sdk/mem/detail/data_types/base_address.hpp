#ifndef CS2_SDK_BASE_ADDRESS_HPP
#define CS2_SDK_BASE_ADDRESS_HPP

// clang-format off
namespace sdk::detail {
	template < typename _addr_t >
		requires std::is_unsigned_v< _addr_t >
	struct base_address_t {
	protected:
		_addr_t m_addr{};
	public:
		ALWAYS_INLINE constexpr base_address_t( ) = default;

		ALWAYS_INLINE constexpr base_address_t( const _addr_t addr ) : m_addr{ addr } {}

		template < typename _ptr_t >
		   requires std::is_pointer_v< _ptr_t >
	   ALWAYS_INLINE base_address_t( const _ptr_t ptr ) : m_addr{ reinterpret_cast< _addr_t >( ptr ) } {}

		ALWAYS_INLINE constexpr operator _addr_t( ) const { return m_addr; }

		template < typename _ret_t >
		   requires std::is_pointer_v< _ret_t >
	   ALWAYS_INLINE _ret_t as( ) const { return reinterpret_cast< _ret_t >( m_addr ); }

		ALWAYS_INLINE constexpr base_address_t< _addr_t >& self_offset( const std::ptrdiff_t offset ) {
			m_addr += offset;

			return *this;
		}

		ALWAYS_INLINE base_address_t< _addr_t >& self_deref( std::size_t count = 1u ) {
			for ( ; m_addr && count; --count )
				m_addr = *as< _addr_t* >( );

			return *this;
		}

		ALWAYS_INLINE base_address_t< _addr_t >& self_rel( const std::ptrdiff_t offset = 0x3, const std::size_t size = 7u ) {
			m_addr += *reinterpret_cast< std::int32_t* >( m_addr + offset ) + size;

			return *this;
		}

		ALWAYS_INLINE base_address_t< _addr_t >& self_find_byte(
			const std::uint8_t byte, const std::size_t max_region, const bool up
		) {
			for ( auto i = m_addr; i && ( up ? ( m_addr - i ) : ( i - m_addr ) ) < max_region; up ? --i : ++i ) {
				if ( *reinterpret_cast< std::uint8_t* >( i ) != byte )
					continue;

				m_addr = i;

				break;
			}

			return *this;
		}

		ALWAYS_INLINE constexpr base_address_t< _addr_t > offset( const std::ptrdiff_t offset ) const {
			auto ret = *this;

			return ret.self_offset( offset );
		}

		ALWAYS_INLINE base_address_t< _addr_t > deref( const std::size_t count = 1u ) const {
			auto ret = *this;

			return ret.self_deref( count );
		}

		ALWAYS_INLINE base_address_t< _addr_t > rel( const std::ptrdiff_t offset = 0x3, const std::size_t size = 7u ) const {
			auto ret = *this;

			return ret.self_rel( offset, size );
		}

		ALWAYS_INLINE base_address_t< _addr_t > find_byte(
			const std::uint8_t byte, const std::size_t max_region, const bool up
		) const {
			auto ret = *this;

			return ret.self_find_byte( byte, max_region, up );
		}
	};
}
// clang-format on

#endif