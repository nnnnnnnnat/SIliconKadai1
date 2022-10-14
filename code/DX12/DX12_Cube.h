//==============================================================================
/// Filename: DX12_Cube.h
/// Description: DX12�L���[�u
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

#include "../../framework.h"

#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include "d3dx12.h"

#pragma comment(lib , "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

struct TexRGBA {
    unsigned char R , G , B , A;
};

class DX12Cube {
public:
    //-----------------------------------------------------------------------------
    // private method

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
    // private method

    ComPtr<ID3D12RootSignature> m_pRootSignature;
    ComPtr<ID3D12PipelineState> m_pPipelineState;
    ComPtr<ID3D12Resource> m_pVertexBuffer;
    ComPtr<ID3D12Resource> m_pIndexBuffer;
    ComPtr<ID3D12Resource> m_pConstantBuffer;
    ComPtr<ID3D12Resource> m_pTextureUproadHeap;
    ComPtr<ID3D12DescriptorHeap> m_pShaderResourceViewHeap;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
    ShaderObject m_vertexShader;
    ShaderObject m_pixelShader;
    std::vector<TexRGBA> textureData;
    //-----------------------------------------------------------------------------
};