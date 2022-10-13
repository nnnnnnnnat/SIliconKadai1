//==============================================================================
/// Filename: DX11_Graphics.cpp
/// Description: DX11初期化
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================


#include "DX11_Graphics.h"

#include <windows.h>

DX11Graphics& DX11Graphics::GetInstance() {
    static DX11Graphics instance;
    return instance;
}

bool DX11Graphics::Init(HWND _hWnd ,
    const unsigned int _width ,
    const unsigned int _height) {

    // ウィンドウサイズ格納
    m_width = _width;
    m_height = _height;

    ComPtr<IDXGIFactory> factory;
    HRESULT hr;

    // ファクトリー作成
    // できなければ そのまま終了

    hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
    if (FAILED(hr)) { // if
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
    hr = D3D11CreateDevice(nullptr ,
        D3D_DRIVER_TYPE_HARDWARE ,
        nullptr ,
        createFlg ,
        featureLevels ,
        _countof(featureLevels) ,
        D3D11_SDK_VERSION ,
        &m_pDevice ,
        &featureLevel ,
        &m_PdeviceContext);
    if (FAILED(hr)) { // if
        return false;
    }

    // スワップチェイン作成
    DXGI_SWAP_CHAIN_DESC scDesc = {}; // スワップチェーンの設定データ
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
    scDesc.Windowed = TRUE; // ウィンドウモード
    scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    // スワップチェインの作成
    hr = factory->CreateSwapChain(m_pDevice.Get() ,
        &scDesc ,
        &m_pSwapChain);
    if (FAILED(hr)) { // if
        return false;
    }

    // スワップチェインからバックバッファリソース取得
    ComPtr<ID3D11Texture2D> p_BackBuffer;

    hr = m_pSwapChain->GetBuffer(0 ,
        IID_PPV_ARGS(&p_BackBuffer));
    if (FAILED(hr)) { // if
        return false;
    }

    // バックバッファリソース用のRTVを作成
    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.Format = scDesc.BufferDesc.Format;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

    hr = m_pDevice->CreateRenderTargetView(p_BackBuffer.Get() ,
        &rtvDesc ,
        &m_pBackBufferView);
    if (FAILED(hr)) { // if
        return false;
    }

    // バックバッファをRTとしてセット
    m_PdeviceContext->OMSetRenderTargets(1 ,
        m_pBackBufferView.GetAddressOf() ,
        nullptr);

    // ビューポートの設定
    m_viewport = { 0.0f ,
        0.0f ,
        (float)m_width ,
        (float)m_height ,
        0.0f ,
        1.0f };

    m_PdeviceContext->RSSetViewports(1 , &m_viewport);

    return true;
}

void DX11Graphics::BeforeRender() {
    // 背景色設定
    m_PdeviceContext->ClearRenderTargetView(m_pBackBufferView.Get() , m_backGroundColor);
}

void DX11Graphics::AfterRender() {
    // バックバッファの内容を画面に表示
    m_pSwapChain->Present(1 , 0);
}

void DX11Graphics::Exit() {
    if (m_PdeviceContext) { // if
        m_PdeviceContext->ClearState();
    }
    if (m_pSwapChain) { // if
        m_pSwapChain->SetFullscreenState(false , nullptr);
    }
}

ID3D11Device* DX11Graphics::GetDXDevice() const {
    return m_pDevice.Get();
}

ID3D11DeviceContext* DX11Graphics::GetDeviceContext() const {
    return m_PdeviceContext.Get();
}

IDXGISwapChain* DX11Graphics::GetSwapChain() const {
    return m_pSwapChain.Get();
}

ID3D11RenderTargetView* DX11Graphics::GetBackBufferPort() const {
    return m_pBackBufferView.Get();
}

D3D11_VIEWPORT DX11Graphics::GetViewPort() const {
    return m_viewport;
}

int DX11Graphics::GetViewPortHeight() const {
    return m_height;
}

int DX11Graphics::GetViewPortWidth() const {
    return m_width;
};




