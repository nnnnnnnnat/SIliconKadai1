#include "Game_LayerMgr.h"
#include "../DX12/DX12_Graphics.h"

GameLayerMgr& GameLayerMgr::GetInstance() {
    static GameLayerMgr instance;
    return instance;
}


bool GameLayerMgr::Init(HWND _hWnd) {
    std::unique_ptr<GameDevice> p;

    p = std::make_unique<DX12Graphics>();

    bool sts = p->Init(_hWnd);

    if (!sts) {
        return false;
    }

    m_deviceHashMap[m_render[(int)RendererType::DirectX12]].swap(p);

    return true;
}
