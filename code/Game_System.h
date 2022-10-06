#pragma once

#include "../framework.h"
#include "DX11_Polygon.h"
#include "DX11_Texture.h"
class GameSystem {
public:
    static GameSystem& GetInstance();
    void Initialize(HWND);
    void Update();
    void Draw();
    void Exit();
private:
    Texture m_tex;
};
