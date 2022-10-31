//==============================================================================
/// Filename: Game_System.cpp
/// Description: �Q�[���̖{�ҏ���
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#include "Game_Main.h"
#include "Game_Input.h"
#include "Game_Camera.h"

#include "../DX11/DX11_Graphics.h"
#include "../DX11/DX11_Sampler.h"

#include "../DX12/DX12_Graphics.h"

#include "../OpenGL/OpenGL_Graphics.h"

GameMain& GameMain::GetInstance() {
    static GameMain instance;
    return instance;
}

void GameMain::Initialize(HWND _hWnd) {

    // �J������� ������
    GameCamera::GetInstance().Init(
        1.0f ,
        1000.0f ,
        45.0f ,
        SCREEN_WIDTH ,
        SCREEN_HEIGHT ,
        XMFLOAT4(0.0f , 3.0f , -5.0f , 0.0f) ,  // �J�������W
        XMFLOAT4(0.0f , 0.0f , 0.0f , 0.0f) ,   // �����_
        XMFLOAT4(0.0f , 1.0f , 0.0f , 0.0f)     // ������x�N�g��
    );

    // �L�[�{�[�h�N���X������ ( Windows �̂ݑΉ�)
    GameInput::GetInstance().Init();

    // �w�肳�ꂽ�f�o�C�X�̏�����
    m_pGameDevice = GameLayerMgr::GetInstance().GetDevicePtr(_hWnd , m_rendererType);

    // �I�u�W�F�N�g������
    m_pCube = GameObject::GetObjectPtr(m_rendererType);
    m_pCube->Init(m_pGameDevice);

    // �E�B���h�E�̃^�C�g�� �ύX
    SetWindowTitle(_hWnd , m_rendererType);

}

void GameMain::Update() {

    // �L�[�{�[�h�̍X�V
    GameInput::GetInstance().Update();

    // ���ɍX�V����������

    m_pCube->Update(m_pGameDevice);

    // ���ɍX�V����������

}

void GameMain::Draw() {

    // API �̕`��O����
    m_pGameDevice->BeforeRender();

    // ���ɕ`�揈��������

    m_pCube->Draw();

    // ���ɕ`�揈��������

    // API �̕`��㏈��
    m_pGameDevice->AfterRender();

}

void GameMain::Release() {

    // �f�o�C�X�̉������
    m_pGameDevice->Release();

    // ���ɉ������������

    m_pCube->Release();

    // ���ɉ������������

}

void GameMain::SetWindowTitle(HWND _hWnd , GameLayerMgr::RendererType _type) {
    // �f�o�C�X�ɉ������e�X�̏�����
    switch (_type) { // switch
    case GameLayerMgr::RendererType::DirectX11:

        SetWindowTextA(_hWnd , "DirectX11�ŕ`�撆");

        break;
    case GameLayerMgr::RendererType::DirectX12:

        SetWindowTextA(_hWnd , "DirectX12�ŕ`�撆");

        break;
    case GameLayerMgr::RendererType::OpenGL:

        SetWindowTextA(_hWnd , "OpenGL�ŕ`�撆");

        break;
    default:

        break;
    }
}
