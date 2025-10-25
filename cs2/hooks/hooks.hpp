#pragma once

namespace hooks {
#pragma region ui
    LRESULT __stdcall wnd_proc( const HWND hwnd, const UINT msg, const WPARAM wparam, const LPARAM lparam );
    inline decltype( &wnd_proc ) o_wnd_proc{};

    HRESULT __stdcall dx11_present( IDXGISwapChain* const swap_chain, const std::uint32_t sync_interval, const std::uint32_t flags );
    inline decltype( &dx11_present ) o_dx11_present{};

    HRESULT __stdcall dx11_resize_buffers( IDXGISwapChain* const swap_chain, const std::uint32_t count,
        const std::uint32_t width, const std::uint32_t height,
        DXGI_FORMAT const format, const std::uint32_t flags
    );
    inline decltype( &dx11_resize_buffers ) o_dx11_resize_buffers{};

    bool __fastcall mouse_input_enabled( const std::uintptr_t rcx );
    inline decltype( &mouse_input_enabled ) o_mouse_input_enabled{};

    void __fastcall is_relative_mouse_mode( valve::input_system_t* const rcx, const bool active );
    inline decltype( &is_relative_mouse_mode ) o_is_relative_mouse_mode{};
#pragma endregion

#pragma region input
    void __fastcall create_move( valve::input_t* const rcx, const int slot, const bool active );
    inline decltype( &create_move ) o_create_move{};
#pragma endregion
}