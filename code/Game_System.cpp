#include "Game_System.h"

#include "DX11_Graphics.h"
#include "DX11_Settransform.h"
#include "DX11_Sampler.h"

GameSystem& GameSystem::GetInstance() {
    static GameSystem instance;
    return instance;
}

void GameSystem::Initialize(HWND _hWnd) {
    bool sts;
    sts = DX11Graphics::GetInstance().Initialize(_hWnd , 1280 , 720);
    if (!sts) {
        MessageBox(NULL , "DX11Graphics Init" , "Error" , MB_OK);
    }
    sts = DX11SetTransform::GetInstance()->Init();
    if (!sts) {
        MessageBox(NULL , "DX11SetTransform Init" , "Error" , MB_OK);
    }

    DX11Sampler::GetInstance()->Init();
    DX11Sampler::GetInstance()->Set(DX11Sampler::Sampler_Mode::WRAP);

    DirectX::XMFLOAT4 color = { 1.0f , 0.0f , 0.0f , 1.0f };
    m_tex.Init(0.5f , 0.5f);
}

void GameSystem::Update() {

}

void GameSystem::Draw() {
    // �w�i�F�ݒ�
    float color[4] = { 1.0f , 0.2f , 0.2f , 1.0f };
    DX11Graphics::GetInstance().GetDeviceContext()->ClearRenderTargetView(DX11Graphics::GetInstance().GetBackBufferPort() , color);

    m_tex.Draw();

    // �o�b�N�o�b�t�@�̓��e����ʂɕ\��
    DX11Graphics::GetInstance().GetSwapChain()->Present(1 , 0);
}

void GameSystem::Exit() {
    // Direct3D�C���X�^���X�폜
    DX11Graphics::GetInstance().Exit();

    DX11SetTransform::GetInstance()->Uninit();

}
