#include "../../cs2.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );

namespace hooks {
    LRESULT __stdcall wnd_proc( const HWND hwnd, const UINT msg, const WPARAM wparam, const LPARAM lparam ) {
        if ( msg == WM_KEYUP
            && wparam == VK_INSERT )
            g_menu->main( ).m_hidden ^= 1;

        o_is_relative_mouse_mode( valve::g_input_system,
            g_menu->main( ).m_hidden ? g_menu->main( ).m_active : false
        );

        if ( !g_menu->main( ).m_hidden ) {
            ImGui_ImplWin32_WndProcHandler( hwnd, msg, wparam, lparam );

            return 1;
        }

        return o_wnd_proc( hwnd, msg, wparam, lparam );
    }

    HRESULT __stdcall dx11_present( IDXGISwapChain* const swap_chain, const std::uint32_t sync_interval, const std::uint32_t flags ) {
        g_render->process_frame( swap_chain );

        ImGui_ImplDX11_NewFrame( );
        ImGui_ImplWin32_NewFrame( );

        ImGui::NewFrame( );

        g_menu->run( );

        ImGui::Render( );

        g_render->device_ctx( )->OMSetRenderTargets( 1u, &g_render->render_view( ), 0 );

        ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );

        return o_dx11_present( swap_chain, sync_interval, flags );
    }

    HRESULT __stdcall dx11_resize_buffers( IDXGISwapChain* const swap_chain, const std::uint32_t count,
       const std::uint32_t width, const std::uint32_t height,
       DXGI_FORMAT const format, const std::uint32_t flags
   ) {
        ImGui_ImplDX11_InvalidateDeviceObjects( );

        g_render->process_frame( swap_chain, false );

        ImGui_ImplDX11_CreateDeviceObjects( );

        return o_dx11_resize_buffers( swap_chain, count, width, height, format, flags );
    }

    bool __fastcall mouse_input_enabled( const std::uintptr_t rcx ) {
        if ( !g_menu->main( ).m_hidden )
            return false;

        return o_mouse_input_enabled( rcx );
    }

    void __fastcall is_relative_mouse_mode( valve::input_system_t* const rcx, const bool active ) {
        g_menu->main( ).m_active = active;

        if ( !g_menu->main( ).m_hidden )
            return o_is_relative_mouse_mode( rcx, false );

        return o_is_relative_mouse_mode( rcx, active );
    }
}