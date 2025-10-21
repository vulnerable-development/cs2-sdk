#include "../../cs2.hpp"

int __stdcall DllMain( _In_ HINSTANCE instance, _In_ DWORD reason, [[maybe_unused]] _In_ LPVOID reserved ) {
	if ( reason != DLL_PROCESS_ATTACH )
		return 0;

	DisableThreadLibraryCalls( instance );

	// clang-format off
	std::jthread{ [ ] ( ) { g_ctx->init( ); } }.detach( );
	// clang-format on

	return 1;
}

void c_ctx::init( ) const { }