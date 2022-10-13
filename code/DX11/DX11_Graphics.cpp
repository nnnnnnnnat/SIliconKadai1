//==============================================================================
/// Filename: DX11_Graphics.cpp
/// Description: DX11������
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

    // �E�B���h�E�T�C�Y�i�[
    m_width = _width;
    m_height = _height;

    ComPtr<IDXGIFactory> factory;
    HRESULT hr;

    // �t�@�N�g���[�쐬
    // �ł��Ȃ���� ���̂܂܏I��

    hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
    if (FAILED(hr)) { // if
        return false;
    }

    // ���\�[�X����
    unsigned int createFlg = 0;

#ifdef _DEBUG
    // DEBUG�r���h���ɂ�DirectX3D�̃f�o�b�O��L���ɂ���
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

    // �f�o�C�X�ƃf�o�C�X�R���e�N�X�g���쐬
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

    // �X���b�v�`�F�C���쐬
    DXGI_SWAP_CHAIN_DESC scDesc = {}; // �X���b�v�`�F�[���̐ݒ�f�[�^
    scDesc.BufferDesc.Width = _width; // ��ʂ̕�
    scDesc.BufferDesc.Height = _height; // ��ʂ̍���
    scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // �o�b�t�@�̌`��
    scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    scDesc.BufferDesc.RefreshRate.Numerator = 0;
    scDesc.BufferDesc.RefreshRate.Denominator = 1;
    scDesc.SampleDesc.Count = 1; // MSAA�͎g�p���Ȃ�
    scDesc.SampleDesc.Quality = 0; // MSAA�͎g�p���Ȃ�
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // �o�b�t�@�̎g�p���@
    scDesc.BufferCount = 2; // �o�b�t�@�̐�
    scDesc.OutputWindow = _hWnd;
    scDesc.Windowed = TRUE; // �E�B���h�E���[�h
    scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    // �X���b�v�`�F�C���̍쐬
    hr = factory->CreateSwapChain(m_pDevice.Get() ,
        &scDesc ,
        &m_pSwapChain);
    if (FAILED(hr)) { // if
        return false;
    }

    // �X���b�v�`�F�C������o�b�N�o�b�t�@���\�[�X�擾
    ComPtr<ID3D11Texture2D> p_BackBuffer;

    hr = m_pSwapChain->GetBuffer(0 ,
        IID_PPV_ARGS(&p_BackBuffer));
    if (FAILED(hr)) { // if
        return false;
    }

    // �o�b�N�o�b�t�@���\�[�X�p��RTV���쐬
    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.Format = scDesc.BufferDesc.Format;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

    hr = m_pDevice->CreateRenderTargetView(p_BackBuffer.Get() ,
        &rtvDesc ,
        &m_pBackBufferView);
    if (FAILED(hr)) { // if
        return false;
    }

    // �o�b�N�o�b�t�@��RT�Ƃ��ăZ�b�g
    m_PdeviceContext->OMSetRenderTargets(1 ,
        m_pBackBufferView.GetAddressOf() ,
        nullptr);

    // �r���[�|�[�g�̐ݒ�
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
    // �w�i�F�ݒ�
    m_PdeviceContext->ClearRenderTargetView(m_pBackBufferView.Get() , m_backGroundColor);
}

void DX11Graphics::AfterRender() {
    // �o�b�N�o�b�t�@�̓��e����ʂɕ\��
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




