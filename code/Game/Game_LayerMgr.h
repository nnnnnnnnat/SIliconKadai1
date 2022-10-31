//==============================================================================
/// Filename: Game_LayerMgr.h
/// Description: 各APIの描画に必要なものを集めたクラス
/// Copyright (C) Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

#include "Game_Device.h"

#include "../Application/Win_Framework.h"

class GameLayerMgr : private NonCopyable {
public:

    enum class RendererType {
        DirectX11 ,
        DirectX12 ,
        OpenGL ,
        max ,
    };

    //-----------------------------------------------------------------------------
    // public method

    //-----------------------------------------------------------------------------
    /// インスタンス取得
    /// 
    /// \return GameLayerMgr&
    //-----------------------------------------------------------------------------
    static GameLayerMgr& GetInstance();

    //-----------------------------------------------------------------------------
    /// 指定したAPIのデバイスを取得
    /// 
    /// \param  [in]    _hWnd   ウィンドウハンドル
    /// \param  [in]    _type   使用するAPIの種類
    /// 
    /// \return GameDevice*
    //-----------------------------------------------------------------------------
    GameDevice* GetDevicePtr(
        /*[in]*/ HWND _hWnd ,
        /*[in]*/ RendererType _type);

    //-----------------------------------------------------------------------------
};
