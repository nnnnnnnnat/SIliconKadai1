//==============================================================================
/// Filename: Game_System.cpp
/// Description: �Q�[���̖{�ҏ���
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#include "Game_System.h"

#include "DX11_Graphics.h"
#include "DX11_Sampler.h"

GameSystem& GameSystem::GetInstance() {
    static GameSystem instance;
    return instance;
}

void GameSystem::Initialize(HWND _hWnd) {
    // �ϐ��錾
    bool sts;

    // ����������
    sts = DX11Graphics::GetInstance().Init(_hWnd , 1280 , 720);
    if (!sts) {
        MessageBox(NULL , "DX11Graphics Init" , "Error" , MB_OK);
    }

    // �T���v���[�ݒ�
    DX11Sampler::GetInstance()->Init();
    DX11Sampler::GetInstance()->Set(DX11Sampler::Sampler_Mode::WRAP);

    // �L���[�u������
    m_cube.Init(0.5f , 0.5f , 0.5f);
}

void GameSystem::Update() {
    // �X�V���������ɏ���

    // �X�V���������ɏ���
}

void GameSystem::Draw() {

    // �w�i�F�ݒ�
    float color[4] = { 0.2f , 0.2f , 0.2f , 1.0f };
    DX11Graphics::GetInstance().GetDeviceContext()->ClearRenderTargetView(DX11Graphics::GetInstance().GetBackBufferPort() , color);

    // �`�揈�������ɏ���

    m_cube.Draw();

    // �`�揈�������ɏ���

    // �o�b�N�o�b�t�@�̓��e����ʂɕ\��
    DX11Graphics::GetInstance().GetSwapChain()->Present(1 , 0);

}

void GameSystem::Exit() {
    // Direct3D�C���X�^���X�폜
    DX11Graphics::GetInstance().Exit();
}
