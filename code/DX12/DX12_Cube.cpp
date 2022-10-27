//==============================================================================
// Filename: DX12_Cube.cpp
// Description: DX12用 キューブ作成クラス
// Copyright (C) Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#include "DX12_Cube.h"

#include "../Game/Game_Device.h"

#include <iostream>

using namespace std;
using namespace DirectX;

//-----------------------------------------------------------------------------
/// 初期化
/// 
/// \param [in] _pDevice 表示するAPIのデバイスのポインタ
/// 
/// \return bool
//-----------------------------------------------------------------------------
bool DX12Cube::Init(
    /*[in]*/ GameDevice* _pDevice) {

    InitCube();

    m_pDevice = dynamic_cast<DX12Graphics*>( _pDevice )->GetDevice();
    m_pCommandList = dynamic_cast<DX12Graphics*>( _pDevice )->GetCommandList();

    // 頂点バッファの生成
    {
        // 頂点データ
        Vertex vertices[36] = {};
        for (int i = 0; i < GetCubeVertex().size(); i++) {
            vertices[i] = GetCubeVertex()[i];
        }

        // ヒーププロパティ
        D3D12_HEAP_PROPERTIES prop = {};

        prop.Type = D3D12_HEAP_TYPE_UPLOAD;
        prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        prop.CreationNodeMask = 1;
        prop.VisibleNodeMask = 1;

        // リソースの設定
        D3D12_RESOURCE_DESC desc = {};

        desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Alignment = 0;
        desc.Width = sizeof(vertices);
        desc.Height = 1;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = 1;
        desc.Format = DXGI_FORMAT_UNKNOWN;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        desc.Flags = D3D12_RESOURCE_FLAG_NONE;

        // リソースを生成
        auto hr = m_pDevice->CreateCommittedResource(
            &prop ,
            D3D12_HEAP_FLAG_NONE ,
            &desc ,
            D3D12_RESOURCE_STATE_GENERIC_READ ,
            nullptr ,
            IID_PPV_ARGS(&m_pVertexBuffer));

        if (FAILED(hr)) {
            MessageBoxA(nullptr , "CreateCommittedResource" , "" , MB_OK);
            return false;
        }

        // マッピング
        void* ptr = nullptr;
        hr = m_pVertexBuffer->Map(0 , nullptr , &ptr);

        if (FAILED(hr)) {
            return false;
        }

        // 頂点データをマッピング先に設定
        memcpy(ptr , vertices , sizeof(vertices));

        // マッピング解除
        m_pVertexBuffer->Unmap(0 , nullptr);

        // 頂点バッファビューの設定
        m_vertexBufferView.BufferLocation = m_pVertexBuffer->GetGPUVirtualAddress();
        m_vertexBufferView.SizeInBytes = static_cast<UINT>( sizeof(vertices) );
        m_vertexBufferView.StrideInBytes = static_cast<UINT>( sizeof(Vertex) );
    }

    // インデックスバッファの生成
    {
        uint32_t indices[36];
        for (int i = 0; i < 36; i++) {
            indices[i] = i;
        }

        // ヒーププロパティ
        D3D12_HEAP_PROPERTIES prop = {};

        prop.Type = D3D12_HEAP_TYPE_UPLOAD;
        prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        prop.CreationNodeMask = 1;
        prop.VisibleNodeMask = 1;

        // リソースの設定
        D3D12_RESOURCE_DESC desc = {};

        desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Alignment = 0;
        desc.Width = sizeof(Transform);
        desc.Height = 1;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = 1;
        desc.Format = DXGI_FORMAT_UNKNOWN;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        desc.Flags = D3D12_RESOURCE_FLAG_NONE;

        // リソースの生成
        auto hr = m_pDevice->CreateCommittedResource(
            &prop ,
            D3D12_HEAP_FLAG_NONE ,
            &desc ,
            D3D12_RESOURCE_STATE_GENERIC_READ ,
            nullptr ,
            IID_PPV_ARGS(&m_pIndexBuffer));

        if (FAILED(hr)) {
            MessageBoxA(nullptr , "CreateCommittedResource" , "" , MB_OK);
            return false;
        }

        // マッピング
        void* ptr = nullptr;
        hr = m_pIndexBuffer->Map(0 , nullptr , &ptr);

        if (FAILED(hr)) {
            MessageBoxA(nullptr , "Map" , "" , MB_OK);
            return false;
        }

        // インデックスデータをマッピング先に設定
        memcpy(ptr , indices , sizeof(indices));

        // マッピング解除
        m_pIndexBuffer->Unmap(0 , nullptr);

        // インデックスバッファビューの設定
        m_indexBufferView.BufferLocation = m_pIndexBuffer->GetGPUVirtualAddress();
        m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
        m_indexBufferView.SizeInBytes = sizeof(indices);


    }

    // 定数バッファ用ディスクリプタヒープの生成
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        desc.NumDescriptors = 1 * FRAME_COUNT;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        desc.NodeMask = 0;

        auto hr = m_pDevice->CreateDescriptorHeap(
            &desc ,
            IID_PPV_ARGS(&m_pHeapCBV));

        if (FAILED(hr)) {
            MessageBoxA(nullptr , "CreateDescriptorHeap" , "" , MB_OK);
            return false;
        }
    }

    // 定数バッファの生成
    {
        // ヒーププロパティ
        D3D12_HEAP_PROPERTIES prop = {};

        prop.Type = D3D12_HEAP_TYPE_UPLOAD;
        prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        prop.CreationNodeMask = 1;
        prop.VisibleNodeMask = 1;

        // リソースの設定
        D3D12_RESOURCE_DESC desc = {};

        desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Alignment = 0;
        desc.Width = sizeof(Transform);
        desc.Height = 1;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = 1;
        desc.Format = DXGI_FORMAT_UNKNOWN;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        desc.Flags = D3D12_RESOURCE_FLAG_NONE;

        auto incrementSize = m_pDevice->GetDescriptorHandleIncrementSize(
            D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
        );

        for (auto i = 0; i < FRAME_COUNT; ++i) {
            // リソースの生成
            auto hr = m_pDevice->CreateCommittedResource(
                &prop ,
                D3D12_HEAP_FLAG_NONE ,
                &desc ,
                D3D12_RESOURCE_STATE_GENERIC_READ ,
                nullptr ,
                IID_PPV_ARGS(&m_pConstantBuffer[i]));

            if (FAILED(hr)) {
                MessageBoxA(nullptr , "CreateCommittedResource" , "" , MB_OK);
                return false;
            }

            auto address = m_pConstantBuffer[i]->GetGPUVirtualAddress();
            auto handleCPU = m_pHeapCBV->GetCPUDescriptorHandleForHeapStart();
            auto handleGPU = m_pHeapCBV->GetGPUDescriptorHandleForHeapStart();

            handleCPU.ptr += incrementSize * 1;
            handleGPU.ptr += incrementSize * 1;

            // 定数バッファビューの設定
            m_constantBufferView[i].HandleCPU = handleCPU;
            m_constantBufferView[i].HandleGPU = handleGPU;
            m_constantBufferView[i].Desc.BufferLocation = address;
            m_constantBufferView[i].Desc.SizeInBytes = sizeof(Transform);

            // 定数バッファビューを生成
            m_pDevice->CreateConstantBufferView(
                &m_constantBufferView[i].Desc ,
                handleCPU);

            // マッピング
            hr = m_pConstantBuffer[i]->Map(
                0 ,
                nullptr ,
                reinterpret_cast<void**>( &m_constantBufferView[i].pBuffer ));

            if (FAILED(hr)) {
                MessageBoxA(nullptr , "Map" , "" , MB_OK);
                return false;
            }


        }
    }

    // ルートシグネイチャの生成
    {
        auto flag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
        flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

        // ルートパラメータの設定
        D3D12_ROOT_PARAMETER param[2] = {};

        param[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
        param[0].Descriptor.ShaderRegister = 0;
        param[0].Descriptor.RegisterSpace = 0;
        param[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

        D3D12_DESCRIPTOR_RANGE range = {};

        range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        range.NumDescriptors = 1;
        range.BaseShaderRegister = 0;
        range.RegisterSpace = 0;
        range.OffsetInDescriptorsFromTableStart = 0;

        param[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        param[1].DescriptorTable.NumDescriptorRanges = 1;
        param[1].DescriptorTable.pDescriptorRanges = &range;
        param[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

        // スタティックサンプラーの設定
        D3D12_STATIC_SAMPLER_DESC sampler = {};

        sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
        sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
        sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
        sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
        sampler.MipLODBias = D3D12_DEFAULT_MIP_LOD_BIAS;
        sampler.MaxAnisotropy = 1;
        sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
        sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
        sampler.MinLOD = -D3D12_FLOAT32_MAX;
        sampler.MaxLOD = +D3D12_FLOAT32_MAX;
        sampler.ShaderRegister = 0;
        sampler.RegisterSpace = 0;
        sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

        // ルートシグネイチャの設定
        D3D12_ROOT_SIGNATURE_DESC desc = {};

        desc.NumParameters = 2;
        desc.NumStaticSamplers = 1;
        desc.pParameters = param;
        desc.pStaticSamplers = &sampler;
        desc.Flags = flag;

        ID3DBlob* pBlob;
        ID3DBlob* pErrorBlob;

        // シリアライズ
        auto hr = D3D12SerializeRootSignature(
            &desc ,
            D3D_ROOT_SIGNATURE_VERSION_1_0 ,
            &pBlob ,
            &pErrorBlob);

        if (FAILED(hr)) {
            MessageBoxA(nullptr , "D3D12SerializeRootSignature" , "" , MB_OK);
            return false;
        }

        // ルートシグネイチャの生成
        hr = m_pDevice->CreateRootSignature(
            0 ,
            pBlob->GetBufferPointer() ,
            pBlob->GetBufferSize() ,
            IID_PPV_ARGS(&m_pRootSignature));

        if (FAILED(hr)) {
            MessageBoxA(nullptr , "CreateRootSignature" , "" , MB_OK);
            return false;
        }
    }

    // パイプラインステートの生成
    {
        D3D12_INPUT_ELEMENT_DESC layout[] = {
            { "POSITION" , 0 , DXGI_FORMAT_R32G32B32_FLOAT , 0 , D3D12_APPEND_ALIGNED_ELEMENT , D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0 } ,
            { "COLOR" , 0 , DXGI_FORMAT_R32G32B32A32_FLOAT , 0 , D3D12_APPEND_ALIGNED_ELEMENT , D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0 } ,
            { "TEXCOORD" , 0 , DXGI_FORMAT_R32G32_FLOAT , 0 , D3D12_APPEND_ALIGNED_ELEMENT , D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0 }
        };

        // ラスタライザーステートの設定
        D3D12_RASTERIZER_DESC descRS;

        descRS.FillMode = D3D12_FILL_MODE_SOLID;
        descRS.CullMode = D3D12_CULL_MODE_NONE;
        descRS.FrontCounterClockwise = FALSE;
        descRS.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
        descRS.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
        descRS.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        descRS.DepthClipEnable = FALSE;
        descRS.MultisampleEnable = FALSE;
        descRS.AntialiasedLineEnable = FALSE;
        descRS.ForcedSampleCount = 0;
        descRS.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

        // 深度ステンシルステートの設定
        D3D12_DEPTH_STENCIL_DESC descDSS = {};

        descDSS.DepthEnable = TRUE;
        descDSS.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        descDSS.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
        descDSS.StencilEnable = FALSE;

        // レンダーターゲットのブレンド設定
        D3D12_RENDER_TARGET_BLEND_DESC descRTBS =
        {
            FALSE , FALSE ,
            D3D12_BLEND_ONE , D3D12_BLEND_ZERO , D3D12_BLEND_OP_ADD ,
            D3D12_BLEND_ONE , D3D12_BLEND_ZERO , D3D12_BLEND_OP_ADD ,
            D3D12_LOGIC_OP_NOOP ,
            D3D12_COLOR_WRITE_ENABLE_ALL
        };

        // ブレンドステートの設定
        D3D12_BLEND_DESC descBS;

        descBS.AlphaToCoverageEnable = FALSE;
        descBS.IndependentBlendEnable = FALSE;

        for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
            descBS.RenderTarget[i] = descRTBS;
        }

        ID3DBlob* pVSBlob;
        ID3DBlob* pPSBlob;

        // 頂点シェーダ読み込み
        auto hr = D3DReadFileToBlob(L"shader/SimpleTexVS.cso" , &pVSBlob);

        if (FAILED(hr)) {
            MessageBoxA(nullptr , "D3DReadFileToBlob VS" , "" , MB_OK);
            return false;
        }

        // ピクセルシェーダ読み込み
        hr = D3DReadFileToBlob(L"shader/SimpleTexPS.cso" , &pPSBlob);

        if (FAILED(hr)) {
            MessageBoxA(nullptr , "D3DReadFileToBlob PS" , "" , MB_OK);
            return false;
        }

        // パイプラインステートの設定
        D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};

        desc.InputLayout = { layout , _countof(layout) };
        desc.pRootSignature = m_pRootSignature;
        desc.VS = { pVSBlob->GetBufferPointer() , pVSBlob->GetBufferSize() };
        desc.PS = { pPSBlob->GetBufferPointer() , pPSBlob->GetBufferSize() };
        desc.RasterizerState = descRS;
        desc.BlendState = descBS;
        desc.DepthStencilState = descDSS;
        desc.SampleMask = UINT_MAX;
        desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        desc.NumRenderTargets = 1;
        desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;

        // パイプラインステートを生成
        hr = m_pDevice->CreateGraphicsPipelineState(
            &desc ,
            IID_PPV_ARGS(&m_pPipelineState));

        if (FAILED(hr)) {
            MessageBoxA(nullptr , "CreateGraphicsPipelineState" , "" , MB_OK);
            return false;
        }
    }

    // ビューポートとシザー矩形の設定
    {
        m_viewport.TopLeftX = 0;
        m_viewport.TopLeftY = 0;
        m_viewport.Width = static_cast<float>( SCREEN_WIDTH );
        m_viewport.Height = static_cast<float>( SCREEN_HEIGHT );
        m_viewport.MinDepth = 0.0f;
        m_viewport.MaxDepth = 1.0f;

        m_scissor.left = 0;
        m_scissor.right = SCREEN_WIDTH;
        m_scissor.top = 0;
        m_scissor.bottom = SCREEN_HEIGHT;
    }

    // テクスチャの生成
    {
        D3D12_RESOURCE_DESC texDesc;
        D3D12_HEAP_PROPERTIES heapProp;
        const UINT64 k_Width = 32;

        ZeroMemory(&texDesc , sizeof(texDesc));
        ZeroMemory(&heapProp , sizeof(heapProp));

        // テクスチャの準備
        heapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
        heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
        heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
        heapProp.CreationNodeMask = 0;
        heapProp.VisibleNodeMask = 0;

        texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        texDesc.Width = k_Width;
        texDesc.Height = k_Width;
        texDesc.DepthOrArraySize = 1;
        texDesc.MipLevels = 1;
        texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        texDesc.SampleDesc.Count = 1;
        texDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

        auto hr = m_pDevice->CreateCommittedResource(
            &heapProp ,
            D3D12_HEAP_FLAG_NONE ,
            &texDesc ,
            D3D12_RESOURCE_STATE_GENERIC_READ ,
            nullptr ,
            IID_PPV_ARGS(&m_texture.pResource));

        if (FAILED(hr)) {
            MessageBoxA(nullptr , "CreateCommittedResource" , "" , MB_OK);
            return false;
        }

        // テクスチャ更新
        D3D12_BOX box = { 0 };
        box.right = k_Width;
        box.bottom = k_Width;
        box.back = 1;
        uint32_t* p = (uint32_t*)malloc(k_Width * k_Width * sizeof(uint32_t));
        const uint32_t red = 0xFFFF0000;
        const uint32_t green = 0xFF00FF00;
        const uint32_t blue = 0xFF0000FF;
        for (int i = 0; i < k_Width * k_Width; i++) {
            if (i / k_Width % 32 == 1 ||
                i / k_Width % 32 == 3 ||
                i / k_Width % 32 == 28 ||
                i / k_Width % 32 == 30) {
                if (p) {
                    p[i] = red;
                }
            }
            else {
                if (i % k_Width == 1 ||
                    i % k_Width == 3 ||
                    i % k_Width == 28 ||
                    i % k_Width == 30) {
                    if (p) {
                        p[i] = green;
                    }
                }
                else {
                    if (p) {
                        p[i] = blue;
                    }
                }
            }
        }

        m_texture.pResource->WriteToSubresource(0 , &box , p , 4 * k_Width , 4 * k_Width * k_Width);

        // インクリメントサイズを取得
        auto incrementSize = m_pDevice->GetDescriptorHandleIncrementSize(
            D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        // CPUディスクリプタハンドルとGPUディスクリプタハンドルを取得
        auto handleCPU = m_pHeapCBV->GetCPUDescriptorHandleForHeapStart();
        auto handleGPU = m_pHeapCBV->GetGPUDescriptorHandleForHeapStart();

        handleCPU.ptr += incrementSize * 1;
        handleGPU.ptr += incrementSize * 1;

        m_texture.HandleCPU = handleCPU;
        m_texture.HandleGPU = handleGPU;

        // テクスチャの構成設定を取得
        auto textureDesc = m_texture.pResource->GetDesc();

        // シェーダーリソースビューの設定
        D3D12_SHADER_RESOURCE_VIEW_DESC viewDesc = {};

        viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        viewDesc.Format = textureDesc.Format;
        viewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        viewDesc.Texture2D.MostDetailedMip = 0;
        viewDesc.Texture2D.MipLevels = textureDesc.MipLevels;
        viewDesc.Texture2D.PlaneSlice = 0;
        viewDesc.Texture2D.ResourceMinLODClamp = 0.0f;

        // シェーダーリソースビューを生成
        m_pDevice->CreateShaderResourceView(
            m_texture.pResource , &viewDesc , handleCPU);
    }
    return true;
}

void DX12Cube::Update(GameDevice* _pDevice) {
    RotateMatrix();
    m_frameIndex = dynamic_cast<DX12Graphics*>( _pDevice )->GetFrameIndex();
    DirectX::XMMATRIX m_mat = XMMatrixIdentity();
    DirectX::XMFLOAT4X4 m_mtx = GetMatrix();
    m_mat = XMLoadFloat4x4(&m_mtx);
    m_constantBufferView[m_frameIndex].pBuffer->World = m_mat;

}

void DX12Cube::Draw() {

    for (auto i = 0; i < FRAME_COUNT; ++i) {

        m_constantBufferView[i].pBuffer->View = GameCamera::GetInstance().GetViewMatrix();
        m_constantBufferView[i].pBuffer->Proj = GameCamera::GetInstance().GetProjectionMatrix();

    }

    m_pCommandList->SetGraphicsRootSignature(m_pRootSignature);
    m_pCommandList->SetDescriptorHeaps(1 , &m_pHeapCBV);
    m_pCommandList->SetGraphicsRootConstantBufferView(0 , m_constantBufferView[m_frameIndex].Desc.BufferLocation);
    m_pCommandList->SetGraphicsRootDescriptorTable(1 , m_texture.HandleGPU);
    m_pCommandList->SetPipelineState(m_pPipelineState);

    m_pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pCommandList->IASetVertexBuffers(0 , 1 , &m_vertexBufferView);
    m_pCommandList->IASetIndexBuffer(&m_indexBufferView);
    m_pCommandList->RSSetViewports(1 , &m_viewport);
    m_pCommandList->RSSetScissorRects(1 , &m_scissor);

    m_pCommandList->DrawIndexedInstanced(36 , 1 , 0 , 0 , 0);
}

void DX12Cube::Release() {
    for (auto i = 0; i < FRAME_COUNT; ++i) {
        if (m_pConstantBuffer[i] != nullptr) {
            m_pConstantBuffer[i]->Unmap(0 , nullptr);
            memset(&m_constantBufferView[i] , 0 , sizeof(m_constantBufferView[i]));
        }
    }
    m_pRootSignature->Release();
    m_pRootSignature = nullptr;
}

void DX12Cube::SetPosition(const float x) {
    m_constantBufferView->pBuffer->World = XMMatrixTranslation(x , 0 , 0);
}
