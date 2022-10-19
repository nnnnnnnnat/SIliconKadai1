//==============================================================================
/// Filename: Game_System.cpp
/// Description: ゲームの本編処理
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#include "Game_System.h"
#include "Game/Game_Input.h"

#include "DX11/DX11_Graphics.h"
#include "DX11/DX11_Sampler.h"

#include "DX12/DX12_Graphics.h"

GameSystem& GameSystem::GetInstance() {
    static GameSystem instance;
    return instance;
}

void GameSystem::Initialize(HWND _hWnd) {
    // 変数宣言
    bool sts;

    GameLayerMgr::GetInstance().Init(_hWnd);

    m_pGameDevice = GameLayerMgr::GetInstance().GetDevicePtr(GameLayerMgr::RendererType::DirectX11);

    DX11Sampler::GetInstance()->Init(dynamic_cast<DX11Graphics*>( m_pGameDevice )->GetDXDevice());
    DX11Sampler::GetInstance()->Set(dynamic_cast<DX11Graphics*>( m_pGameDevice )->GetDeviceContext() , DX11Sampler::Sampler_Mode::WRAP);

    // キューブ初期化
    m_dx11Cube.Init(dynamic_cast<DX11Graphics*>( m_pGameDevice )->GetDXDevice() , dynamic_cast<DX11Graphics*>( m_pGameDevice )->GetDeviceContext());

    m_pGameDevice = GameLayerMgr::GetInstance().GetDevicePtr(GameLayerMgr::RendererType::DirectX12);

    m_dx12Cube.Init(dynamic_cast<DX12Graphics*>( m_pGameDevice )->GetDevice() , dynamic_cast<DX12Graphics*>( m_pGameDevice )->GetCommandList());

    switch (m_rendererType) {
    case GameLayerMgr::RendererType::DirectX11:

        m_pGameDevice = GameLayerMgr::GetInstance().GetDevicePtr(GameLayerMgr::RendererType::DirectX11);

        break;
    case GameLayerMgr::RendererType::DirectX12:

        m_pGameDevice = GameLayerMgr::GetInstance().GetDevicePtr(GameLayerMgr::RendererType::DirectX12);

        break;
    }
    GameInput::Init();
}

void GameSystem::Update() {
    GameInput::Update();
    if (GameInput::GetKeyTrigger(VK_F1)) {
        if (m_rendererType != GameLayerMgr::RendererType::DirectX11) {
            m_pGameDevice->BeforeRender();
            m_pGameDevice->AfterRender();
            m_pGameDevice = GameLayerMgr::GetInstance().GetDevicePtr(GameLayerMgr::RendererType::DirectX11);
            m_rendererType = GameLayerMgr::RendererType::DirectX11;
            m_pGameDevice->BeforeRender();
            m_pGameDevice->AfterRender();
        }
    }
    if (GameInput::GetKeyTrigger(VK_F2)) {
        m_rendererType = GameLayerMgr::RendererType::DirectX12;
        m_pGameDevice = GameLayerMgr::GetInstance().GetDevicePtr(GameLayerMgr::RendererType::DirectX12);
        m_rendererType = GameLayerMgr::RendererType::DirectX12;
    }
    switch (m_rendererType) {
    case GameLayerMgr::RendererType::DirectX11:

        break;
    case GameLayerMgr::RendererType::DirectX12:

        m_dx12Cube.Update(dynamic_cast<DX12Graphics*>( m_pGameDevice )->SystemGetFrameIndex());

        break;
    }
}

void GameSystem::Draw() {


    // 描画処理を↓に書く

    switch (m_rendererType) {
    case GameLayerMgr::RendererType::DirectX11:
        m_pGameDevice->BeforeRender();
        m_dx11Cube.Draw();
        m_pGameDevice->AfterRender();

        break;
    case GameLayerMgr::RendererType::DirectX12:
        m_pGameDevice->BeforeRender();
        m_dx12Cube.Draw();
        m_pGameDevice->AfterRender();

        break;
    }

    // 描画処理を↑に書く

}

void GameSystem::Exit() {

    GameLayerMgr::GetInstance().Release();

}
