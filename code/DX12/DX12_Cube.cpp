//==============================================================================
/// Filename: DX12_Cube.cpp
/// Description: DX12キューブ
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#include "DX12_Cube.h"

#include "DX12_Graphics.h"

using namespace DirectX;

struct DX12Vertex {
    DirectX::XMFLOAT3 Pos;
    DirectX::XMFLOAT3 Color;
    DirectX::XMFLOAT2 Tex;
};


DX12Vertex g_cubeVertices[] = {
    { XMFLOAT3(-1.0f , -1.0f , -1.0f) , XMFLOAT3(0.0f , 0.0f , 0.0f) , XMFLOAT2(0.0f , 0.0f) } ,
    { XMFLOAT3(-1.0f , -1.0f , 1.0f) , XMFLOAT3(0.0f , 0.0f , 1.0f) , XMFLOAT2(1.0f , 0.0f) } ,
    { XMFLOAT3(-1.0f , 1.0f , -1.0f) , XMFLOAT3(0.0f , 1.0f , 0.0f) , XMFLOAT2(0.0f , 1.0f) } ,
    { XMFLOAT3(-1.0f , 1.0f , 1.0f) , XMFLOAT3(0.0f , 1.0f , 1.0f) , XMFLOAT2(1.0f , 1.0f) } ,
    { XMFLOAT3(1.0f , -1.0f , -1.0f) , XMFLOAT3(1.0f , 0.0f , 0.0f) , XMFLOAT2(1.0f , 1.0f) } ,
    { XMFLOAT3(1.0f , -1.0f , 1.0f) , XMFLOAT3(1.0f , 0.0f , 1.0f) , XMFLOAT2(0.0f , 1.0f) } ,
    { XMFLOAT3(1.0f , 1.0f , -1.0f) , XMFLOAT3(1.0f , 1.0f , 0.0f) , XMFLOAT2(1.0f , 0.0f) } ,
    { XMFLOAT3(1.0f , 1.0f , 1.0f) , XMFLOAT3(1.0f , 1.0f , 1.0f) , XMFLOAT2(0.0f , 0.0f) }

};
uint16_t g_cubeIndices[] = {
    0 , 2 , 1 , // -x
    1 , 2 , 3 ,

    4 , 5 , 6 , // +x
    5 , 7 , 6 ,

    0 , 1 , 5 , // -y
    0 , 5 , 4 ,

    2 , 6 , 7 , // +y
    2 , 7 , 3 ,

    0 , 4 , 6 , // -z
    0 , 6 , 2 ,

    1 , 3 , 7 , // +z
    1 , 7 , 5 ,
};

bool DX12Cube::Init(
    const float _width ,
    const float _height ,
    const float _depth) {

    ID3D12Device* dev = DX12Graphics::GetInstance().GetDevice();
    HRESULT hr;

    // RootSignature作成
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    ZeroMemory(&rootSignatureDesc , sizeof(rootSignatureDesc));
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    D3D12_ROOT_PARAMETER rootParameters[1];
    rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
    rootParameters[0].Descriptor.ShaderRegister = 0;
    rootParameters[0].Descriptor.RegisterSpace = 0;
    rootSignatureDesc.NumParameters = 1;
    rootSignatureDesc.pParameters = rootParameters;
    ComPtr<ID3DBlob> rootSigBlob , errorBlob;
    hr = D3D12SerializeRootSignature(&rootSignatureDesc , D3D_ROOT_SIGNATURE_VERSION_1 , rootSigBlob.GetAddressOf() , errorBlob.GetAddressOf());
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "D3D12SerializeRootSignature" , "" , MB_OK);
    }
    hr = dev->CreateRootSignature(0 , rootSigBlob->GetBufferPointer() , rootSigBlob->GetBufferSize() , IID_PPV_ARGS(m_pRootSignature.GetAddressOf()));
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "CreateRootSignature" , "" , MB_OK);
    }

    // コンパイル済みシェーダーの読み込み.
    // コンパイルそのものはVisualStudioのビルド時にやる.
    FILE* fpVS = nullptr;
    fopen_s(&fpVS , "VertexShader.cso" , "rb");
    if (!fpVS) {
        MessageBox(nullptr , "fopen_s" , "" , MB_OK);
        return FALSE;
    }
    fseek(fpVS , 0 , SEEK_END);
    m_vertexShader.size = ftell(fpVS); rewind(fpVS);
    m_vertexShader.binaryPtr = malloc(m_vertexShader.size);
    fread(m_vertexShader.binaryPtr , 1 , m_vertexShader.size , fpVS);
    fclose(fpVS); fpVS = nullptr;
    FILE* fpPS = nullptr;
    fopen_s(&fpPS , "PixelShader.cso" , "rb");
    if (!fpPS) {
        MessageBox(nullptr , "fopen_s" , "" , MB_OK);
        return FALSE;
    }
    fseek(fpPS , 0 , SEEK_END);
    m_pixelShader.size = ftell(fpPS); rewind(fpPS);
    m_pixelShader.binaryPtr = malloc(m_pixelShader.size);
    fread(m_pixelShader.binaryPtr , 1 , m_pixelShader.size , fpPS);
    fclose(fpPS); fpPS = nullptr;

    // セマンティクス設定
    D3D12_INPUT_ELEMENT_DESC inputElementDesc[] = {
        { "POSITION" , 0 , DXGI_FORMAT_R32G32B32_FLOAT , 0 , 0 , D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0 } ,
        { "COLOR" , 0 , DXGI_FORMAT_R32G32B32_FLOAT , 0 , 12 , D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0 } ,
        { "TEXCOORD" , 0 , DXGI_FORMAT_R32G32_FLOAT , 0 , 0 , D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0 } ,
    };

    // PipelineStateオブジェクトの作成.
    D3D12_GRAPHICS_PIPELINE_STATE_DESC descPipelineState;
    descPipelineState = DX12Util::CreateGraphicsPipelineStateDesc(
        m_pRootSignature.Get() ,
        m_vertexShader.binaryPtr , m_vertexShader.size ,
        m_pixelShader.binaryPtr , m_pixelShader.size ,
        inputElementDesc ,
        _countof(inputElementDesc));

    descPipelineState.DepthStencilState.DepthEnable = TRUE;
    descPipelineState.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
    descPipelineState.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    descPipelineState.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    hr = dev->CreateGraphicsPipelineState(&descPipelineState , IID_PPV_ARGS(m_pPipelineState.GetAddressOf()));
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "CreateGraphicsPipelineState " , "" , MB_OK);
        return false;
    }

    // 頂点データの作成
    m_pVertexBuffer = DX12Util::CreateVertexBuffer(
        dev ,
        sizeof(g_cubeVertices) ,
        D3D12_HEAP_TYPE_UPLOAD ,
        D3D12_RESOURCE_STATE_GENERIC_READ);
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "CreateCommittedResource " , "" , MB_OK);
        return false;
    }

    // 頂点データの書き込み
    void* mapped = nullptr;
    hr = m_pVertexBuffer->Map(0 , nullptr , &mapped);
    if (SUCCEEDED(hr)) { // if
        memcpy(mapped , g_cubeVertices , sizeof(g_cubeVertices));
        m_pVertexBuffer->Unmap(0 , nullptr);
    }
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "Map " , "" , MB_OK);
        return false;
    }
    m_vertexBufferView.BufferLocation = m_pVertexBuffer->GetGPUVirtualAddress();
    m_vertexBufferView.StrideInBytes = sizeof(DX12Vertex);
    m_vertexBufferView.SizeInBytes = sizeof(g_cubeVertices);

    // インデックスバッファ生成
    m_pIndexBuffer = DX12Util::CreateIndexBuffer(
        dev ,
        sizeof(g_cubeIndices) ,
        D3D12_HEAP_TYPE_UPLOAD ,
        D3D12_RESOURCE_STATE_GENERIC_READ);
    if (!m_pIndexBuffer) { // if
        MessageBox(nullptr , "!m_pIndexBuffer " , "" , MB_OK);
        return false;
    }
    hr = m_pIndexBuffer->Map(0 , nullptr , &mapped);
    if (SUCCEEDED(hr)) { // if
        memcpy(mapped , g_cubeIndices , sizeof(g_cubeIndices));
        m_pIndexBuffer->Unmap(0 , nullptr);
    }

    m_indexBufferView.BufferLocation = m_pIndexBuffer->GetGPUVirtualAddress();
    m_indexBufferView.SizeInBytes = sizeof(g_cubeIndices);
    m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;

    // コンスタントバッファをXMFLOAT4x4分確保
    m_pConstantBuffer = DX12Util::CreateConstantBuffer(dev , sizeof(DirectX::XMFLOAT4X4));

    if (!m_pConstantBuffer) { // if
        MessageBox(nullptr , "!m_pConstantBuffer " , "" , MB_OK);
        return false;
    }
    hr = m_pConstantBuffer->Map(0 , nullptr , &mapped);
    if (SUCCEEDED(hr)) {
        DirectX::XMFLOAT4X4 mtx;
        DirectX::XMStoreFloat4x4(&mtx , DirectX::XMMatrixIdentity());
        memcpy(mapped , &mtx , sizeof(mtx));
        m_pConstantBuffer->Unmap(0 , nullptr);
    }

    D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
    srvHeapDesc.NumDescriptors = 1;
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    hr = dev->CreateDescriptorHeap(&srvHeapDesc , IID_PPV_ARGS(m_pShaderResourceViewHeap.GetAddressOf()));
    if (FAILED(hr)) {
        MessageBox(nullptr , "CreateDescriptorHeap" , "" , MB_OK);
        return false;
    }

    {

        textureData.resize(256 * 256);
        for (auto& rgba : textureData) {
            rgba.R = 255;
            rgba.G = 0;
            rgba.B = 0;
            rgba.A = 255;
        }

        D3D12_HEAP_PROPERTIES heapProperties = {};
        heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;
        heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
        heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
        heapProperties.CreationNodeMask = 0;
        heapProperties.VisibleNodeMask = 0;

        D3D12_RESOURCE_DESC resDesc = {};
        resDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        resDesc.Width = 256;
        resDesc.Height = 256;
        resDesc.DepthOrArraySize = 1;
        resDesc.SampleDesc.Count = 1;
        resDesc.SampleDesc.Quality = 0;
        resDesc.MipLevels = 1;
        resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

        hr = dev->CreateCommittedResource(
            &heapProperties ,
            D3D12_HEAP_FLAG_NONE ,
            &resDesc ,
            D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE ,
            nullptr ,
            IID_PPV_ARGS(m_pTextureUproadHeap.GetAddressOf())
        );
        if (FAILED(hr)) {
            MessageBox(nullptr , "CreateCommittedResource" , "" , MB_OK);
            return false;
        }

        hr = m_pTextureUproadHeap->WriteToSubresource(
            0 ,
            nullptr ,
            textureData.data() ,
            sizeof(TexRGBA) * 256 ,
            sizeof(TexRGBA) * textureData.size()
        );
        if (FAILED(hr)) {
            MessageBox(nullptr , "WriteToSubresource" , "" , MB_OK);
            return false;
        }

        // m_pShaderResourceViewHeap
        D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
    }

    return true;
}

void DX12Cube::Draw() {

    int windowWidth = DX12Graphics::GetInstance().GetWindowWidth();
    int windowHeight = DX12Graphics::GetInstance().GetWindowHeight();
    static int s_count = 0;
    // ビューおよびプロジェクション行列を準備.
    DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(
        DirectX::XMVECTOR{ 3.0f , 3.0f , -5.0f } ,
        DirectX::XMVECTOR{ 0.0f , 0.0f , 0.0f } ,
        DirectX::XMVECTOR{ 0.0f , 1.0f , 0.0f }
    );
    float fov = DirectX::XM_PI / 3.0f;
    DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(
        fov ,
        float(windowWidth) / float(windowHeight) ,
        0.1f , 100.0f
    );

    // 回転させる
    DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
    world = DirectX::XMMatrixRotationY(s_count * 0.02f);
    DirectX::XMMATRIX wvp = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(world , view) , proj);
    void* pCB;
    m_pConstantBuffer->Map(0 , nullptr , &pCB);
    if (pCB) {
        DirectX::XMStoreFloat4x4((DirectX::XMFLOAT4X4*)pCB , wvp);
        m_pConstantBuffer->Unmap(0 , nullptr);
    }

    ID3D12GraphicsCommandList* pCommandList = DX12Graphics::GetInstance().GetCommandList();
    D3D12_VIEWPORT viewPort = DX12Graphics::GetInstance().GetViewPort();
    D3D12_RECT rect = DX12Graphics::GetInstance().GetRect();

    pCommandList->SetGraphicsRootSignature(m_pRootSignature.Get());
    pCommandList->SetPipelineState(m_pPipelineState.Get());
    pCommandList->SetGraphicsRootConstantBufferView(0 , m_pConstantBuffer->GetGPUVirtualAddress());
    //pCommandList->SetGraphicsRootDescriptorTable(0 , m_pShaderResourceViewHeap->GetGPUDescriptorHandleForHeapStart());
    pCommandList->RSSetViewports(1 , &viewPort);
    pCommandList->RSSetScissorRects(1 , &rect);

    // 頂点データをセット.
    pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    pCommandList->IASetVertexBuffers(0 , 1 , &m_vertexBufferView);
    pCommandList->IASetIndexBuffer(&m_indexBufferView);
    pCommandList->DrawIndexedInstanced(36 , 1 , 0 , 0 , 0);

    s_count++;
}
