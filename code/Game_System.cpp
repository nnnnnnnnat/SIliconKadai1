//==============================================================================
/// Filename: Game_System.cpp
/// Description: �Q�[���̖{�ҏ���
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#include "Game_System.h"

#include "DX11/DX11_Graphics.h"
#include "DX11/DX11_Sampler.h"

#include "DX12/DX12_Graphics.h"

GameSystem& GameSystem::GetInstance() {
    static GameSystem instance;
    return instance;
}

void GameSystem::Initialize(HWND _hWnd) {
    // �ϐ��錾
    bool sts;

    switch (m_rendererType) { // switch
    case RendererType::DirectX11:
        // ����������
        sts = DX11Graphics::GetInstance().Init(_hWnd , 1280 , 720);
        if (!sts) {
            MessageBox(NULL , "DX11Graphics Init" , "Error" , MB_OK);
        }

        // �T���v���[�ݒ�
        DX11Sampler::GetInstance()->Init();
        DX11Sampler::GetInstance()->Set(DX11Sampler::Sampler_Mode::WRAP);

        // �L���[�u������
        m_dx11Cube.Init(0.5f , 0.5f , 0.5f);
        break;
    case RendererType::DirectX12:

        DX12Graphics::GetInstance().Init(_hWnd);
        m_dx12Cube.Init();

        break;
    }
}

void GameSystem::Update() {
    switch (m_rendererType) { // switch
    case RendererType::DirectX11:
        // �X�V���������ɏ���

        // �X�V���������ɏ���
        break;
    case RendererType::DirectX12:
        // �X�V���������ɏ���

        // �X�V���������ɏ���
        break;
    }

}

void GameSystem::Draw() {

    switch (m_rendererType) { // switch
    case RendererType::DirectX11:

        DX11Graphics::GetInstance().BeforeRender();

        // �`�揈�������ɏ���

        m_dx11Cube.Draw();

        // �`�揈�������ɏ���

        DX11Graphics::GetInstance().AfterRender();

        break;
    case RendererType::DirectX12:

        DX12Graphics::GetInstance().SystemBeforeRender();

        //// �`�揈�������ɏ���

        m_dx12Cube.Draw();

        //// �`�揈�������ɏ���

        DX12Graphics::GetInstance().SystemAfterRender();

        break;
    }


}

void GameSystem::Exit() {

    switch (m_rendererType) { // switch
    case RendererType::DirectX11:

        DX11Graphics::GetInstance().Exit();

        break;
    case RendererType::DirectX12:

        m_dx12Cube.Release();
        DX12Graphics::GetInstance().Release();

        break;
    }
}
