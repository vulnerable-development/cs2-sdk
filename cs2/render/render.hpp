#pragma once

class c_render {
private:
    void process( ) const;

    ID3D11Device*           m_device{};
    ID3D11DeviceContext*    m_device_ctx{};
    ID3D11RenderTargetView* m_render_view{};

    HWND                    m_hwnd{};
public:
    void process_frame( IDXGISwapChain* const swap_chain, const bool call_once = true );

    ALWAYS_INLINE const auto& device( ) const;

    ALWAYS_INLINE const auto& device_ctx( ) const;

    ALWAYS_INLINE auto& render_view( );
};

inline const auto g_render = std::make_unique< c_render >( );

#include "impl/render.inl"