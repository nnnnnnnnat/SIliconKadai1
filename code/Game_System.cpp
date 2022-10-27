//==============================================================================
/// Filename: Game_System.cpp
/// Description: �Q�[���̖{�ҏ���
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#include "Game_System.h"
#include "Game/Game_Input.h"
#include "Game/Game_Camera.h"

#include "DX11/DX11_Graphics.h"
#include "DX11/DX11_Sampler.h"

#include "DX12/DX12_Graphics.h"

#include "OpenGL/OpenGL_Graphics.h"

GameSystem& GameSystem::GetInstance() {
    static GameSystem instance;
    return instance;
}

void GameSystem::Initialize(HWND _hWnd) {

    GameCamera::GetInstance().Init(
        1.0f ,
        1000.0f ,
        45.0f ,
        SCREEN_WIDTH ,
        SCREEN_HEIGHT ,
        XMFLOAT4(0.0f , 3.0f , -5.0f , 0.0f) ,
        XMFLOAT4(0.0f , 0.0f , 0.0f , 0.0f) ,
        XMFLOAT4(0.0f , 1.0f , 0.0f , 0.0f)
    );

    GameLayerMgr::GetInstance().Init(_hWnd);

    switch (m_rendererType) { // switch
    case GameLayerMgr::RendererType::DirectX11:

        m_pGameDevice = GameLayerMgr::GetInstance().GetDevicePtr(m_rendererType);
        m_cube = new DX11Cube();
        m_cube->Init(m_pGameDevice);
        SetWindowTextA(_hWnd , "DirectX11");

        break;
    case GameLayerMgr::RendererType::DirectX12:

        m_pGameDevice = GameLayerMgr::GetInstance().GetDevicePtr(m_rendererType);
        m_cube = new DX12Cube();
        m_cube->Init(m_pGameDevice);
        SetWindowTextA(_hWnd , "DirectX12");

        break;
    case GameLayerMgr::RendererType::OpenGL:

        m_pGameDevice = GameLayerMgr::GetInstance().GetDevicePtr(m_rendererType);
        m_cube = new OpenGLCube();
        m_cube->Init(m_pGameDevice);
        SetWindowTextA(_hWnd , "OpenGL");

        break;
    default:
        MessageBoxA(nullptr , "������ ���s" , "" , MB_OK);
        break;
    }

    GameInput::Init();
}

void GameSystem::Update() {

    GameInput::Update();

    // ���ɍX�V����������

    m_cube->Update(m_pGameDevice);

    // ���ɍX�V����������

}

void GameSystem::Draw() {

    m_pGameDevice->BeforeRender();

    // ���ɕ`�揈��������

    m_cube->Draw();

    // ���ɕ`�揈��������

    m_pGameDevice->AfterRender();

}

void GameSystem::Exit() {

    GameLayerMgr::GetInstance().Release();

}
