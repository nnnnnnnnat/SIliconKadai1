//==============================================================================
/// Filename: Game_System.cpp
/// Description: ゲームの本編処理
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
    // 変数宣言
    bool sts;

    // 初期化処理
    sts = DX11Graphics::GetInstance().Init(_hWnd , 1280 , 720);
    if (!sts) {
        MessageBox(NULL , "DX11Graphics Init" , "Error" , MB_OK);
    }

    // サンプラー設定
    DX11Sampler::GetInstance()->Init();
    DX11Sampler::GetInstance()->Set(DX11Sampler::Sampler_Mode::WRAP);

    // キューブ初期化
    m_cube.Init(0.5f , 0.5f , 0.5f);
}

void GameSystem::Update() {
    // 更新処理を↓に書く

    // 更新処理を↑に書く
}

void GameSystem::Draw() {

    // 背景色設定
    float color[4] = { 0.2f , 0.2f , 0.2f , 1.0f };
    DX11Graphics::GetInstance().GetDeviceContext()->ClearRenderTargetView(DX11Graphics::GetInstance().GetBackBufferPort() , color);

    // 描画処理を↓に書く

    m_cube.Draw();

    // 描画処理を↑に書く

    // バックバッファの内容を画面に表示
    DX11Graphics::GetInstance().GetSwapChain()->Present(1 , 0);

}

void GameSystem::Exit() {
    // Direct3Dインスタンス削除
    DX11Graphics::GetInstance().Exit();
}
