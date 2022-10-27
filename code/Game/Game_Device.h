#pragma once

#include <Windows.h>
#include <DirectXMath.h>

class GameDevice {
public:
    virtual bool Init(HWND) = 0;
    virtual void Release() = 0;
    virtual void BeforeRender() = 0;
    virtual void AfterRender() = 0;
protected:
    float m_backColor[4] = { 0.5f , 0.5f , 0.5f , 1.0f };
};

