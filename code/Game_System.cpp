//==============================================================================
/// Filename: Game_System.cpp
/// Description: ゲームの本編処理
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
    // 変数宣言
    bool sts;

    switch (m_rendererType) { // switch
    case RendererType::DirectX11:
        // 初期化処理
        sts = DX11Graphics::GetInstance().Init(_hWnd , 1280 , 720);
        if (!sts) {
            MessageBox(NULL , "DX11Graphics Init" , "Error" , MB_OK);
        }

        // サンプラー設定
        DX11Sampler::GetInstance()->Init();
        DX11Sampler::GetInstance()->Set(DX11Sampler::Sampler_Mode::WRAP);

        // キューブ初期化
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
        // 更新処理を↓に書く

        // 更新処理を↑に書く
        break;
    case RendererType::DirectX12:
        // 更新処理を↓に書く

        // 更新処理を↑に書く
        break;
    }

}

void GameSystem::Draw() {

    switch (m_rendererType) { // switch
    case RendererType::DirectX11:

        DX11Graphics::GetInstance().BeforeRender();

        // 描画処理を↓に書く

        m_dx11Cube.Draw();

        // 描画処理を↑に書く

        DX11Graphics::GetInstance().AfterRender();

        break;
    case RendererType::DirectX12:

        DX12Graphics::GetInstance().SystemBeforeRender();

        //// 描画処理を↓に書く

        m_dx12Cube.Draw();

        //// 描画処理を↑に書く

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
