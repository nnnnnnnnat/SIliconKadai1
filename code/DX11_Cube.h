//==============================================================================
/// Filename: DX11_Cube.h
/// Description: �{�b�N�X�\������N���X
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

#include "../framework.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <array>
#include <wrl/client.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

using Microsoft::WRL::ComPtr;

typedef std::array<Vertex , 36> VertexCube;

class DX11Cube {
public:
    //-----------------------------------------------------------------------------
    // public methods

    //-----------------------------------------------------------------------------
    /// ������
    /// 
    /// \param [in] _width ���̑傫��
    /// \param [in] _height �c�̑傫��
    /// \param [in] _depth ���s��
    /// 
    /// \return bool
    //-----------------------------------------------------------------------------
    bool Init(
        /*[in]*/ const float _width ,
        /*[in]*/ const float _height ,
        /*[in]*/ const float _depth);

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
    VertexCube m_cube;
    //-----------------------------------------------------------------------------

    ///<
    /// m_pVertexShader ���_�V�F�[�_�[
    /// m_pPixelShader �s�N�Z���V�F�[�_�[
    /// m_pInputLayout ���̓��C�A�E�g
    /// m_pVertexBuffer ���_�o�b�t�@
    /// m_pShaderResourceView �V�F�[�_�[���\�[�X�r���[
    /// m_cube �L���[�u�\��������ϐ�
    ///<

    //-----------------------------------------------------------------------------
};
