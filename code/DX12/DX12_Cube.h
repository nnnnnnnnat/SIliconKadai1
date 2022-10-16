#pragma once
//==============================================================================
// Filename: DX12_Cube.h
// Description: CubeClass for DirectX12
// Copyright (C) Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#include <DirectXMath.h>
#include <d3dcompiler.h>

#include "DX12_Graphics.h"

template<typename T>
struct ConstantBufferView {
    D3D12_CONSTANT_BUFFER_VIEW_DESC Desc;
    D3D12_CPU_DESCRIPTOR_HANDLE HandleCPU;
    D3D12_GPU_DESCRIPTOR_HANDLE HandleGPU;
    T* pBuffer;

    ///<
    /// Desc �萔�o�b�t�@�̍\���ݒ�
    /// HandleCPU CPU�f�B�X�N���v�^�n���h��
    /// HandleGPU GPU�f�B�X�N���v�^�n���h��
    /// pBuffer �o�b�t�@�擪�ւ̃|�C���^
    ///<
};

struct alignas( 256 ) Transform {
    DirectX::XMMATRIX World;
    DirectX::XMMATRIX View;
    DirectX::XMMATRIX Proj;

    ///<
    /// World ���[���h�s��
    /// View �r���[�s��
    /// Proj �v���W�F�N�V�����s��
    ///<
};

struct Texture {
    ID3D12Resource* pResource;
    D3D12_CPU_DESCRIPTOR_HANDLE HandleCPU;
    D3D12_GPU_DESCRIPTOR_HANDLE HandleGPU;

    ///<
    /// pResource ���\�[�X
    /// HandleCPU CPU�f�B�X�N���v�^�n���h��
    /// HandleGPU GPU�f�B�X�N���v�^�n���h��
    ///<
};

class DX12Cube {
public:
    //-----------------------------------------------------------------------------
    // public methods

    //-----------------------------------------------------------------------------
    /// ������
    /// 
    /// \return bool
    //-----------------------------------------------------------------------------
    bool Init();

    //-----------------------------------------------------------------------------
    /// �X�V����
    /// 
    /// uint32_t [in] frameindex �t���[���ԍ�
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Update(
        /*[in]*/ const uint32_t frameindex);

    //-----------------------------------------------------------------------------
    /// �`��
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Draw();

    //-----------------------------------------------------------------------------
    /// �������
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Release();

    //-----------------------------------------------------------------------------
    /// ���W�i�[ (X�����W�̂ݍ���͑Ή�)
    /// 
    /// \param x X�����W
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void SetPosition(
        /*[in]*/ const float x);
    //-----------------------------------------------------------------------------
private:
    //-----------------------------------------------------------------------------
    // private varable

    ID3D12DescriptorHeap* m_pHeapCBV;
    ID3D12Resource* m_pIndexBuffer;
    ID3D12Resource* m_pVertexBuffer;
    ID3D12Resource* m_pConstantBuffer[FRAME_COUNT];
    ID3D12RootSignature* m_pRootSignature;
    ID3D12PipelineState* m_pPipelineState;

    uint32_t m_frameIndex;

    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
    D3D12_VIEWPORT m_viewport;
    D3D12_RECT m_scissor;
    ConstantBufferView<Transform> m_constantBufferView[FRAME_COUNT];
    Texture m_texture;

    float m_rotateAngle;

    ///<
    /// m_pHeapCBV �f�B�X�N���v�^�q�[�v
    /// m_pIndexBuffer �C���f�b�N�X�o�b�t�@
    /// m_pVertexBuffer ���_�o�b�t�@
    /// m_pConstantBuffer �萔�o�b�t�@
    /// m_pRootSignature ���[�g�V�O�l�C�`��
    /// m_pPipelineState �p�C�v���C���X�e�[�g
    /// 
    /// m_frameIndex �t���[���ԍ�
    /// 
    /// m_indexBufferView �C���f�b�N�X�o�b�t�@�r���[
    /// m_vertexBufferView ���_�o�b�t�@�r���[
    /// m_viewport �r���[�|�[�g
    /// m_scissor �V�U�[��`
    /// m_constantBufferView �萔�o�b�t�@�r���[
    /// m_texture �e�N�X�`���f�[�^
    /// 
    /// m_rotateAngle ��]�p
    //-----------------------------------------------------------------------------

};