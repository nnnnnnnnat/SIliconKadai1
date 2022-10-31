//==============================================================================
/// Filename: Game_LayerMgr.cpp
/// Description: �eAPI�̕`��ɕK�v�Ȃ��̂��W�߂��N���X
/// Copyright (C) Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#include "Game_LayerMgr.h"
#include "../DX11/DX11_Graphics.h"
#include "../DX12/DX12_Graphics.h"
#include "../OpenGL/OpenGL_Graphics.h"

GameLayerMgr& GameLayerMgr::GetInstance() {
    static GameLayerMgr instance;
    return instance;
}

GameDevice* GameLayerMgr::GetDevicePtr(HWND _hWnd , RendererType _type) {

    GameDevice* pGameDevice = nullptr;
    bool sts = false;
    switch (_type) { // switch
    case RendererType::DirectX11:
        // DX11 ������
        pGameDevice = new DX11Graphics();
        sts = pGameDevice->Init(_hWnd);
        if (!sts) { // if
            MessageBoxA(NULL , "DX11Graphics Init" , "Error" , MB_OK);
        }
        break;
    case RendererType::DirectX12:
        // DX12 ������
        pGameDevice = new DX12Graphics();
        sts = pGameDevice->Init(_hWnd);
        if (!sts) { // if
            MessageBoxA(NULL , "DX12Graphics Init" , "Error" , MB_OK);
        }
        break;
    case RendererType::OpenGL:
        // OpenGL ������
        pGameDevice = new OpenGLGraphics();
        sts = pGameDevice->Init(_hWnd);
        if (!sts) { // if
            MessageBoxA(NULL , "OpenGLGraphics Init" , "Error" , MB_OK);
        }
        break;
    }

    return pGameDevice;
}
