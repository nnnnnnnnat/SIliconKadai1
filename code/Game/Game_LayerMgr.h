//==============================================================================
/// Filename: Game_LayerMgr.h
/// Description: 各APIの描画に必要なものを集めたクラス
/// Copyright (C) Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "Game_Device.h"

#include "../Utilities.h"

class GameLayerMgr : private NonCopyable {
public:

    enum class RendererType {
        DirectX11 ,
        DirectX12 ,
        OpenGL ,
        max ,
    };

    static GameLayerMgr& GetInstance();
    GameDevice* GetDevicePtr(HWND , RendererType);
private:
};
