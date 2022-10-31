//==============================================================================
/// Filename: Game_LayerMgr.h
/// Description: �eAPI�̕`��ɕK�v�Ȃ��̂��W�߂��N���X
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
    /// �C���X�^���X�擾
    /// 
    /// \return GameLayerMgr&
    //-----------------------------------------------------------------------------
    static GameLayerMgr& GetInstance();

    //-----------------------------------------------------------------------------
    /// �w�肵��API�̃f�o�C�X���擾
    /// 
    /// \param  [in]    _hWnd   �E�B���h�E�n���h��
    /// \param  [in]    _type   �g�p����API�̎��
    /// 
    /// \return GameDevice*
    //-----------------------------------------------------------------------------
    GameDevice* GetDevicePtr(
        /*[in]*/ HWND _hWnd ,
        /*[in]*/ RendererType _type);

    //-----------------------------------------------------------------------------
};
