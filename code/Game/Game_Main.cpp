//==============================================================================
/// Filename: Game_System.cpp
/// Description: ゲームの本編処理
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

    // カメラ情報 初期化
    GameCamera::GetInstance().Init(
        1.0f ,
        1000.0f ,
        45.0f ,
        SCREEN_WIDTH ,
        SCREEN_HEIGHT ,
        XMFLOAT4(0.0f , 3.0f , -5.0f , 0.0f) ,  // カメラ座標
        XMFLOAT4(0.0f , 0.0f , 0.0f , 0.0f) ,   // 注視点
        XMFLOAT4(0.0f , 1.0f , 0.0f , 0.0f)     // 上向きベクトル
    );

    // キーボードクラス初期化 ( Windows のみ対応)
    GameInput::GetInstance().Init();

    // 指定されたデバイスの初期化
    m_pGameDevice = GameLayerMgr::GetInstance().GetDevicePtr(_hWnd , m_rendererType);

    // オブジェクト初期化
    m_pCube = GameObject::GetObjectPtr(m_rendererType);
    m_pCube->Init(m_pGameDevice);

    // ウィンドウのタイトル 変更
    SetWindowTitle(_hWnd , m_rendererType);

}

void GameMain::Update() {

    // キーボードの更新
    GameInput::GetInstance().Update();

    // ↓に更新処理を入れる

    m_pCube->Update(m_pGameDevice);

    // ↑に更新処理を入れる

}

void GameMain::Draw() {

    // API の描画前処理
    m_pGameDevice->BeforeRender();

    // ↓に描画処理を入れる

    m_pCube->Draw();

    // ↑に描画処理を入れる

    // API の描画後処理
    m_pGameDevice->AfterRender();

}

void GameMain::Release() {

    // デバイスの解放処理
    m_pGameDevice->Release();

    // ↓に解放処理を入れる

    m_pCube->Release();

    // ↑に解放処理を入れる

}

void GameMain::SetWindowTitle(HWND _hWnd , GameLayerMgr::RendererType _type) {
    // デバイスに応じた各々の初期化
    switch (_type) { // switch
    case GameLayerMgr::RendererType::DirectX11:

        SetWindowTextA(_hWnd , "DirectX11で描画中");

        break;
    case GameLayerMgr::RendererType::DirectX12:

        SetWindowTextA(_hWnd , "DirectX12で描画中");

        break;
    case GameLayerMgr::RendererType::OpenGL:

        SetWindowTextA(_hWnd , "OpenGLで描画中");

        break;
    default:

        break;
    }
}
