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
    bool Init(HWND);
    void Release();
    GameDevice* GetDevicePtr(RendererType);
private:

    std::string m_renderName[(int)RendererType::max] = {
        "DirectX11" ,
        "DirectX12" ,
        "OpenGL" ,
    };

    std::unordered_map<std::string , std::unique_ptr<GameDevice>> m_deviceHashMap;
};
