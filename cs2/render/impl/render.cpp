#include "../../cs2.hpp"

void c_render::process( ) const {
    ImGui::CreateContext( );
    ImGui::StyleColorsClassic( );

    ImGui::GetStyle( ).WindowMinSize = { 450, 350 };

    auto& io = ImGui::GetIO( );

    io.IniFilename = io.LogFilename = nullptr;

    ImGui_ImplWin32_Init( m_hwnd );
    ImGui_ImplDX11_Init( m_device, m_device_ctx );
}

void c_render::process_frame( IDXGISwapChain* swap_chain, const bool call_once ) {
    DXGI_SWAP_CHAIN_DESC desc{};
    swap_chain->GetDesc( &desc );

    m_hwnd = desc.OutputWindow;

    swap_chain->GetDevice( IID_PPV_ARGS( &m_device ) );
    m_device->GetImmediateContext( &m_device_ctx );

    if ( call_once ) {
        static std::once_flag once_flag{};
        std::call_once( once_flag, [ & ] ( ) { process( ); } );
    }

    ID3D11Texture2D* buffer{};
    swap_chain->GetBuffer( 0u, IID_PPV_ARGS( &buffer ) );

    D3D11_RENDER_TARGET_VIEW_DESC view_desc{};
    view_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

    m_device->CreateRenderTargetView( buffer, &view_desc, &m_render_view );

    RELEASE_D3D( buffer );
}