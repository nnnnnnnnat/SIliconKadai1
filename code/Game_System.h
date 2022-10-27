//==============================================================================
/// Filename: Game_System.h
/// Description: ゲームの本編処理
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

#include "../framework.h"

#include "DX11/DX11_Cube.h"
#include "DX12/DX12_Cube.h"
#include "OpenGL/OpenGL_Cube.h"

#include "Game/Game_Device.h"
#include "Game/Game_LayerMgr.h"

class GameSystem {
public:
    //-----------------------------------------------------------------------------
    // public methods

    //-----------------------------------------------------------------------------
    /// インスタンス受け取り
    ///
    /// \return GameSystem&
    //-----------------------------------------------------------------------------
    static GameSystem& GetInstance();

    //-----------------------------------------------------------------------------
    /// 初期化
    ///
    /// \param[in] _hWnd ウィンドウハンドル
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Initialize(HWND _hWnd);

    //-----------------------------------------------------------------------------
    /// 更新処理
    ///
    /// \return void
    //-----------------------------------------------------------------------------
    void Update();

    //-----------------------------------------------------------------------------
    /// 描画処理
    ///
    /// \return void
    //-----------------------------------------------------------------------------
    void Draw();

    //-----------------------------------------------------------------------------
    /// 解放処理   
    ///
    /// \return void
    //-----------------------------------------------------------------------------
    void Exit();

    //-----------------------------------------------------------------------------
private:
    //-----------------------------------------------------------------------------
    // private methods

    //-----------------------------------------------------------------------------
    GameCube* m_cube = nullptr;
    GameDevice* m_pGameDevice = nullptr;
    GameLayerMgr::RendererType m_rendererType = GameLayerMgr::RendererType::DirectX12;
    //-----------------------------------------------------------------------------

    ///< 
    /// m_dx11Cube DirectX11のキューブを描画する変数
    /// m_dx12Cube DirectX12のキューブを描画する変数
    /// m_rendererType 描画処理のパターン
    ///<

    //-----------------------------------------------------------------------------
};
