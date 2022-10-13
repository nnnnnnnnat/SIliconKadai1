#pragma once

#include <d3d12.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;

class DX12Util {
public:
    // リソース状態変更のための関数.
    static void SetResourceBarrier(
        ID3D12GraphicsCommandList* commandList ,
        ID3D12Resource* resource ,
        D3D12_RESOURCE_STATES before ,
        D3D12_RESOURCE_STATES after);

    // 標準的な GraphicsPipelineStateDescを生成.
    static D3D12_GRAPHICS_PIPELINE_STATE_DESC CreateGraphicsPipelineStateDesc(
        ID3D12RootSignature* pRootSignature ,
        const void* pBinaryVS , int vsSize ,
        const void* pBinaryPS , int psSize ,
        D3D12_INPUT_ELEMENT_DESC* descInputElements ,
        int numInputElements
    );

    static ComPtr<ID3D12Resource> CreateVertexBuffer(
        ID3D12Device* device ,
        int bufferSize ,
        D3D12_HEAP_TYPE type ,
        D3D12_RESOURCE_STATES states
    );
    static ComPtr<ID3D12Resource> CreateIndexBuffer(
        ID3D12Device* device ,
        int bufferSize ,
        D3D12_HEAP_TYPE type ,
        D3D12_RESOURCE_STATES states
    );
    static ComPtr<ID3D12Resource> CreateConstantBuffer(
        ID3D12Device* device ,
        int bufferSize
    );

    static ComPtr<ID3D12Resource> CreateDepthBuffer(
        ID3D12Device* device ,
        int width ,
        int height);
};
