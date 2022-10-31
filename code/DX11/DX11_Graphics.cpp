//==============================================================================
/// Filename: DX11_Graphics.cpp
/// Description: DX11������
/// Copyright (C) Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#include "DX11_Graphics.h"

#include <windows.h>

bool DX11Graphics::Init(HWND _hWnd) {

    ComPtr<IDXGIFactory> factory;
    HRESULT hr;

    // �t�@�N�g���[�쐬
    // �ł��Ȃ���� ���̂܂܏I��

    hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
    if (FAILED(hr)) { // if
        MessageBoxA(nullptr , "CreateDXGIFactory1" , "" , MB_OK);
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
        &m_pDeviceContext);
    if (FAILED(hr)) { // if
        MessageBoxA(nullptr , "D3D11CreateDevice" , "" , MB_OK);
        return false;
    }

    // �X���b�v�`�F�C���쐬
    DXGI_SWAP_CHAIN_DESC scDesc = {}; // �X���b�v�`�F�[���̐ݒ�f�[�^
    scDesc.BufferDesc.Width = SCREEN_WIDTH; // ��ʂ̕�
    scDesc.BufferDesc.Height = SCREEN_HEIGHT; // ��ʂ̍���
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
    scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    // �X���b�v�`�F�C���̍쐬
    hr = factory->CreateSwapChain(m_pDevice.Get() ,
        &scDesc ,
        &m_pSwapChain);
    if (FAILED(hr)) { // if
        MessageBoxA(nullptr , "CreateSwapChain" , "" , MB_OK);
        return false;
    }

    // �X���b�v�`�F�C������o�b�N�o�b�t�@���\�[�X�擾
    ComPtr<ID3D11Texture2D> p_BackBuffer;

    hr = m_pSwapChain->GetBuffer(0 ,
        IID_PPV_ARGS(&p_BackBuffer));
    if (FAILED(hr)) { // if
        MessageBoxA(nullptr , "GetBuffer" , "" , MB_OK);
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
        MessageBoxA(nullptr , "CreateRenderTargetView" , "" , MB_OK);
        return false;
    }

    // �o�b�N�o�b�t�@��RT�Ƃ��ăZ�b�g
    m_pDeviceContext->OMSetRenderTargets(1 ,
        m_pBackBufferView.GetAddressOf() ,
        nullptr);

    {
        // Z�o�b�t�@
        D3D11_TEXTURE2D_DESC depthBufferDesc;
        D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
        D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

        ZeroMemory(&depthBufferDesc , sizeof(depthBufferDesc));

        depthBufferDesc.Width = SCREEN_WIDTH;
        depthBufferDesc.Height = SCREEN_HEIGHT;
        depthBufferDesc.MipLevels = 1;
        depthBufferDesc.ArraySize = 1;
        depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthBufferDesc.SampleDesc.Count = 1;
        depthBufferDesc.SampleDesc.Quality = 0;
        depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthBufferDesc.CPUAccessFlags = 0;
        depthBufferDesc.MiscFlags = 0;

        hr = m_pDevice->CreateTexture2D(&depthBufferDesc , NULL , &m_pDepthStencilBuffer);
        if (FAILED(hr)) { // if
            MessageBoxA(nullptr , "CreateTexture2D" , "" , MB_OK);
            return false;
        }

        // �X�e���V���X�e�[�g�쐬
        // �X�e���V���ݒ�\���̏�����
        ZeroMemory(&depthStencilDesc , sizeof(depthStencilDesc));
        depthStencilDesc.DepthEnable = true;
        depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

        // set up the description of the stencl state
        depthStencilDesc.StencilEnable = true;
        depthStencilDesc.StencilReadMask = 0xFF;
        depthStencilDesc.StencilWriteMask = 0xFF;

        // stencil operation if pixel is front-facing
        depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        // stencil operation if pixel is�@back-facing
        depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        // create the depth stencil state
        hr = m_pDevice->CreateDepthStencilState(&depthStencilDesc , &m_pDepthStencilState);
        if (FAILED(hr)) { // if
            MessageBoxA(nullptr , "CreateDepthStencilState" , "" , MB_OK);
            return false;
        }

        // �f�o�C�X�R���e�L�X�g�փZ�b�g
        m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get() , 1);

        // depthstencilview ������
        ZeroMemory(&depthStencilViewDesc , sizeof(depthStencilViewDesc));
        // set up the depth stencil view description
        depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        depthStencilViewDesc.Texture2D.MipSlice = 0;

        // create the depth stencil view
        hr = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer.Get() , &depthStencilViewDesc , &m_pDepthStencilView);
        if (FAILED(hr)) { // if
            MessageBoxA(nullptr , "CreateDepthStencilView" , "" , MB_OK);
            return false;
        }
    }

    // �r���[�|�[�g�̐ݒ�
    m_viewport = { 0.0f ,
        0.0f ,
        (float)SCREEN_WIDTH ,
        (float)SCREEN_HEIGHT ,
        0.0f ,
        1.0f };

    m_pDeviceContext->RSSetViewports(1 , &m_viewport);

    DX11Sampler::GetInstance()->Init(m_pDevice.Get());
    DX11Sampler::GetInstance()->Set(m_pDeviceContext.Get() , DX11Sampler::Sampler_Mode::WRAP);

    return true;
}

void DX11Graphics::BeforeRender() {
    // �w�i�F�ݒ�
    m_pDeviceContext->ClearRenderTargetView(m_pBackBufferView.Get() , m_backColor);
    // Z�o�b�t�@�N���A
    m_pDeviceContext->ClearDepthStencilView(
        m_pDepthStencilView.Get() ,
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL , 1.0f , 0);
}

void DX11Graphics::AfterRender() {
    // �o�b�N�o�b�t�@�̓��e����ʂɕ\��
    m_pSwapChain->Present(0 , 0);
}

void DX11Graphics::Release() {
    if (m_pDeviceContext) { // if
        m_pDeviceContext->ClearState();
    }
    if (m_pSwapChain) { // if
        m_pSwapChain->SetFullscreenState(false , nullptr);
    }
}

ID3D11Device* DX11Graphics::GetDXDevice() const {
    return m_pDevice.Get();
}

ID3D11DeviceContext* DX11Graphics::GetDeviceContext() const {
    return m_pDeviceContext.Get();
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
