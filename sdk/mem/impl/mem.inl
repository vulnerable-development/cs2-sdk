#ifndef CS2_SDK_MEM_INL
#define CS2_SDK_MEM_INL

#include "../mem.hpp"

// clang-format off
namespace sdk {
	ALWAYS_INLINE void peb_t::for_each_ldr_data_table_entry(
		const std::function< bool( ldr_data_table_entry_t* const ) >& fn, const e_ldr_data_table type
	) const {
		const auto list = &m_ldr->m_module_lists.m_entries[ -type ];

		for ( auto i = list->Flink; i != list; i = i->Flink ) {
			const auto entry = CONTAINING_RECORD( i, ldr_data_table_entry_t, m_links.m_entries[ -type ] );
			if ( !entry )
				continue;

			if ( fn( entry ) )
				break;
		}
	}

	ALWAYS_INLINE peb_t* peb( ) {
		return reinterpret_cast< peb_t* >( __readgsqword( 0x60u ) );
	}
}
// clang-format on

#endif