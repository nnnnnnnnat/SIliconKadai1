#pragma once

#include <Windows.h>

class GameDevice {
public:
    virtual bool Init(HWND) = 0;
    virtual void BeforeRender() = 0;
    virtual void AfterRender() = 0;
};

