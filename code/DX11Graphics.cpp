#include "DX11Graphics.h"

#include <windows.h>

bool DX11Graphics::Initialize(HWND _hWnd , const unsigned int _width , const unsigned int _height) {

    m_width = _width;
    m_height = _height;

    ComPtr<IDXGIFactory> factory;

    // ファクトリー作成
    // できなければ そのまま終了
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory)))) { // if
        return false;
    }

    // リソース生成
    unsigned int createFlg = 0;

#ifdef _DEBUG
    // DEBUGビルド時にはDirectX3Dのデバッグを有効にする
    createFlg |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1 , // DirectX11
        D3D_FEATURE_LEVEL_11_0 , // DirectX11
        D3D_FEATURE_LEVEL_10_1 , // DirectX10
        D3D_FEATURE_LEVEL_10_0 , // DirectX10
        D3D_FEATURE_LEVEL_9_3 , // DirectX9
        D3D_FEATURE_LEVEL_9_2 , // DirectX9
        D3D_FEATURE_LEVEL_9_1 , // DirectX9
    };

    // デバイスとデバイスコンテクストを作成
    D3D_FEATURE_LEVEL featureLevel;
    if (FAILED(D3D11CreateDevice(
        nullptr ,
        D3D_DRIVER_TYPE_HARDWARE ,
        nullptr ,
        createFlg ,
        featureLevels ,
        _countof(featureLevels) ,
        D3D11_SDK_VERSION ,
        &m_device ,
        &featureLevel ,
        &m_deviceContext))) { // if
        return false;
    }

    // スワップチェイン作成
    DXGI_SWAP_CHAIN_DESC scDesc = {};
    scDesc.BufferDesc.Width = _width; // 画面の幅
    scDesc.BufferDesc.Height = _height; // 画面の高さ
    scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // バッファの形式
    scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    scDesc.BufferDesc.RefreshRate.Numerator = 0;
    scDesc.BufferDesc.RefreshRate.Denominator = 1;
    scDesc.SampleDesc.Count = 1; // MSAAは使用しない
    scDesc.SampleDesc.Quality = 0; // MSAAは使用しない
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // バッファの使用方法
    scDesc.BufferCount = 2; // バッファの数
    scDesc.OutputWindow = _hWnd;
    scDesc.Windowed = TRUE;// ウィンドウモード
    scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    // スワップチェインの作成
    if (FAILED(factory->CreateSwapChain(m_device.Get() , &scDesc , &m_swapChain))) {
        return false;
    }

    // スワップチェインからバックバッファリソース取得
    ComPtr<ID3D11Texture2D> p_BackBuffer;
    if (FAILED(m_swapChain->GetBuffer(0 , IID_PPV_ARGS(&p_BackBuffer)))) {
        return false;
    }

    // バックバッファリソース用のRTVを作成
    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.Format = scDesc.BufferDesc.Format;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    if (FAILED(m_device->CreateRenderTargetView(p_BackBuffer.Get() , &rtvDesc , &m_backBufferView))) {
        return false;
    }

    // バックバッファをRTとしてセット
    m_deviceContext->OMSetRenderTargets(1 , m_backBufferView.GetAddressOf() , nullptr);

    // ビューポートの設定
    D3D11_VIEWPORT vp = { 0.0f , 0.0f , (float)m_width , (float)m_height , 0.0f , 1.0f };
    m_deviceContext->RSSetViewports(1 , &vp);

    return true;
}

