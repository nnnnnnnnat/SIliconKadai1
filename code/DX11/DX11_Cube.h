//==============================================================================
/// Filename: DX11_Cube.h
/// Description: �{�b�N�X�\������N���X
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

#include "../../framework.h"

#include "../Game/Game_Camera.h"

#include "../Game/Game_Cube.h"

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

class DX11Cube {
public:
    //-----------------------------------------------------------------------------
    // public methods

    //-----------------------------------------------------------------------------
    /// ������
    /// 
    /// \param [in] _pDev DX11�̃f�o�C�X
    /// \param [in] _pDevContext DX11�̃f�o�C�X�R���e�L�X�g
    /// \param [in] _cube �`�悷��L���[�u�̃|�C���^
    /// 
    /// \return bool
    //-----------------------------------------------------------------------------
    bool Init(
        ID3D11Device* _pDev ,
        ID3D11DeviceContext* _pDevContext ,
        GameCube* _cube);

    //-----------------------------------------------------------------------------
    /// �X�V����
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Update();

    //-----------------------------------------------------------------------------
    /// �`��
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Draw();

    //-----------------------------------------------------------------------------

private:
    //-----------------------------------------------------------------------------
    // private methods

    //-----------------------------------------------------------------------------
    ComPtr<ID3D11VertexShader>	m_pVertexShader = nullptr;
    ComPtr<ID3D11PixelShader>	m_pPixelShader = nullptr;
    ComPtr<ID3D11InputLayout>	m_pInputLayout = nullptr;
    ComPtr<ID3D11Buffer> m_pVertexBuffer = nullptr;
    ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView = nullptr;

    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pDeviceContext = nullptr;

    GameCube* m_cube;
    //-----------------------------------------------------------------------------

    ///<
    /// m_pVertexShader ���_�V�F�[�_�[
    /// m_pPixelShader �s�N�Z���V�F�[�_�[
    /// m_pInputLayout ���̓��C�A�E�g
    /// m_pVertexBuffer ���_�o�b�t�@
    /// m_pShaderResourceView �V�F�[�_�[���\�[�X�r���[
    /// 
    /// m_pDevice�@DirectX11�̃f�o�C�X
    /// m_pDeviceContext DirectX11�̃f�o�C�X�R���e�L�X�g
    /// 
    /// m_cube �L���[�u�\��������ϐ�
    ///<

    //-----------------------------------------------------------------------------
};
