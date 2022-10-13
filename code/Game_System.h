//==============================================================================
/// Filename: Game_System.h
/// Description: �Q�[���̖{�ҏ���
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

#include "../framework.h"

#include "DX11/DX11_Cube.h"
#include "DX12/DX12_Cube.h"

class GameSystem {
public:
    //-----------------------------------------------------------------------------
    // public enum

    enum class RendererType {
        DirectX11 ,
        DirectX12 ,
    };

    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // public methods

    //-----------------------------------------------------------------------------
    /// �C���X�^���X�󂯎��
    ///
    /// \return GameSystem&
    //-----------------------------------------------------------------------------
    static GameSystem& GetInstance();

    //-----------------------------------------------------------------------------
    /// ������
    ///
    /// \param[in] _hWnd �E�B���h�E�n���h��
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Initialize(HWND _hWnd);

    //-----------------------------------------------------------------------------
    /// �X�V����
    ///
    /// \return void
    //-----------------------------------------------------------------------------
    void Update();

    //-----------------------------------------------------------------------------
    /// �`�揈��
    ///
    /// \return void
    //-----------------------------------------------------------------------------
    void Draw();

    //-----------------------------------------------------------------------------
    /// �������   
    ///
    /// \return void
    //-----------------------------------------------------------------------------
    void Exit();

    //-----------------------------------------------------------------------------
private:
    //-----------------------------------------------------------------------------
    // private methods

    //-----------------------------------------------------------------------------
    DX11Cube m_dx11Cube;
    DX12Cube m_dx12Cube;
    RendererType m_rendererType = RendererType::DirectX12;
    //-----------------------------------------------------------------------------

    ///< 
    /// m_cube �L���[�u��`�悷��ϐ�
    /// m_rendererType �`�揈���̃p�^�[��
    ///<

    //-----------------------------------------------------------------------------
};