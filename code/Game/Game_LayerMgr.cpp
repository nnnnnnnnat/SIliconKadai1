#include "Game_LayerMgr.h"
#include "../DX11/DX11_Graphics.h"
#include "../DX12/DX12_Graphics.h"
#include "../OpenGL/OpenGL_Graphics.h"

GameLayerMgr& GameLayerMgr::GetInstance() {
    static GameLayerMgr instance;
    return instance;
}


bool GameLayerMgr::Init(HWND _hWnd) {


    // DX11Device 作成
    {
        std::unique_ptr<GameDevice> p;

        p = std::make_unique<DX11Graphics>();

        bool sts = p->Init(_hWnd);

        if (!sts) {
            if (!sts) {
                MessageBox(NULL , "DX11Graphics Init" , "Error" , MB_OK);
            }
            return false;
        }

        m_deviceHashMap[m_renderName[(int)RendererType::DirectX11]].swap(p);
    }


    // DX12Device 作成
    {
        std::unique_ptr<GameDevice> p;

        p = std::make_unique<DX12Graphics>();

        bool sts = p->Init(_hWnd);

        if (!sts) {
            if (!sts) {
                MessageBox(NULL , "DX12Graphics Init" , "Error" , MB_OK);
            }
            return false;
        }

        m_deviceHashMap[m_renderName[(int)RendererType::DirectX12]].swap(p);
    }

    // OpenGL初期化
    {
        std::unique_ptr<GameDevice> p;

        p = std::make_unique<OpenGLGraphics>();

        bool sts = p->Init(_hWnd);

        if (!sts) {
            if (!sts) {
                MessageBox(NULL , "OpenGLGraphics Init" , "Error" , MB_OK);
            }
            return false;
        }

        m_deviceHashMap[m_renderName[(int)RendererType::OpenGL]].swap(p);
    }

    return true;
}

void GameLayerMgr::Release() {
    for (auto itr = m_deviceHashMap.begin(); itr != m_deviceHashMap.end(); ++itr) {
        ( itr->second )->Release();
    }
}

GameDevice* GameLayerMgr::GetDevicePtr(RendererType _type) {

    // 存在するかを確認
    auto it = m_deviceHashMap.find(m_renderName[static_cast<int>( _type )]);

    // 存在しない場合、nullptrを返す
    if (it == m_deviceHashMap.end()) {
        return nullptr;
    }

    return m_deviceHashMap[m_renderName[static_cast<int>( _type )]].get();
}
