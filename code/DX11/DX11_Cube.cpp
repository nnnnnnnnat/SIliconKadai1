//==============================================================================
/// Filename: DX11_Cube.cpp
/// Description: ボックス表示するクラス
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#include "DX11_Cube.h"
#include "DX11_Graphics.h"

bool DX11Cube::Init(ID3D11Device* _pDev , ID3D11DeviceContext* _pDevContext , GameCube* _cube) {

    HRESULT hr;
    m_pDevice = _pDev;
    m_pDeviceContext = _pDevContext;
    m_cube = _cube;

    //-----------------------------
    // 頂点バッファ作成
    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // デバイスにバインドするときの種類(頂点バッファ、インデックスバッファ、定数バッファなど)
    vbDesc.ByteWidth = sizeof(m_cube->GetCubevertex()); // 作成するバッファのバイトサイズ
    vbDesc.MiscFlags = 0; // その他のフラグ
    vbDesc.StructureByteStride = 0;	// 構造化バッファの場合、その構造体のサイズ
    vbDesc.Usage = D3D11_USAGE_DEFAULT;	// 作成するバッファの使用法
    vbDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initData = { &m_cube->GetCubevertex()[0] , sizeof(m_cube->GetCubevertex()) , 0 }; // 書き込むデータ
    // 頂点バッファの作成
    hr = m_pDevice->CreateBuffer(&vbDesc , &initData , &m_pVertexBuffer);
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "CreateBuffer" , "" , MB_OK);
        return false;
    }

    // 頂点シェーダーを読み込み＆コンパイル
    ComPtr<ID3DBlob> compiledVS;

    hr = D3DCompileFromFile(L"shader/SpriteShader.hlsl" ,
        nullptr ,
        nullptr ,
        "VS" ,
        "vs_5_0" ,
        0 ,
        0 ,
        &compiledVS ,
        nullptr);
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "vs" , "" , MB_OK);
        return false;
    }

    // ピクセルシェーダーを読み込み＆コンパイル
    ComPtr<ID3DBlob> compiledPS;

    hr = D3DCompileFromFile(L"shader/SpriteShader.hlsl" ,
        nullptr ,
        nullptr ,
        "PS" ,
        "ps_5_0" ,
        0 ,
        0 ,
        &compiledPS ,
        nullptr);
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "ps" , "" , MB_OK);
        return false;
    }

    // 頂点シェーダー作成
    hr = m_pDevice->CreateVertexShader(compiledVS->GetBufferPointer() ,
        compiledVS->GetBufferSize() ,
        nullptr ,
        &m_pVertexShader);
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "vx create" , "" , MB_OK);
        return false;
    }

    // ピクセルシェーダー作成
    hr = m_pDevice->CreatePixelShader(compiledPS->GetBufferPointer() ,
        compiledPS->GetBufferSize() ,
        nullptr ,
        &m_pPixelShader);
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "ps create" , "" , MB_OK);
        return false;
    }

    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION" , 0 , DXGI_FORMAT_R32G32B32_FLOAT , 0 , D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 } ,
        { "COLOR" , 0 , DXGI_FORMAT_R32G32B32A32_FLOAT , 0 , D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 } ,
        { "TEXCOORD" , 0 , DXGI_FORMAT_R32G32_FLOAT , 0 , D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 }
    };

    hr = m_pDevice->CreateInputLayout(
        layout ,
        ARRAYSIZE(layout) ,
        compiledVS->GetBufferPointer() ,
        compiledVS->GetBufferSize() ,
        &m_pInputLayout);

    // 頂点インプットレイアウト作成
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "CreateInputLayout" , "" , MB_OK);
        return false;
    }

    // テクスチャ作成

    const int iPixSize = 32;// 縦横ピクセル数

    Microsoft::WRL::ComPtr<ID3D11Texture2D> D3DTexture;
    D3D11_TEXTURE2D_DESC td;
    td.Width = iPixSize;
    td.Height = iPixSize;
    td.MipLevels = 1;
    td.ArraySize = 1;
    td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    td.SampleDesc.Count = 1;
    td.SampleDesc.Quality = 0;
    td.Usage = D3D11_USAGE_DYNAMIC;
    td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    td.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    td.MiscFlags = 0;

    hr = m_pDevice->CreateTexture2D(&td , nullptr , &D3DTexture);
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "CreateTexture2D" , "" , MB_OK);
        return false;
    }

    //テクスチャ書き替え
    D3D11_MAPPED_SUBRESOURCE msr;
    _pDevContext->Map(D3DTexture.Get() , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &msr);

    byte srcData[iPixSize * iPixSize * 4] = { 0 };// ビットマップを黒で初期化
    for (int i = 0; i < iPixSize * iPixSize * 4; i += 4) {
        if (i % 32 >= 0 && i % 32 < 16) {
            if (i % 8 == 0) { // if
                srcData[i] = 255;// Red
                srcData[i + 1] = 255;// Green
                srcData[i + 2] = 255;// Blue
            }
        }
        else if (i % 8 == 4) { // else if
            srcData[i] = 255;// Red
            srcData[i + 1] = 255;// Green
            srcData[i + 2] = 255;// Blue
        }
    }
    memcpy(msr.pData , srcData , sizeof(srcData));

    _pDevContext->Unmap(D3DTexture.Get() , 0);

    //シェーダリソースビューの作成
    D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
    srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srv.Texture2D.MipLevels = 1;
    hr = m_pDevice->CreateShaderResourceView(D3DTexture.Get() , &srv , &m_pShaderResourceView);
    if (FAILED(hr)) {
        MessageBox(nullptr , "CreateShaderResourceView" , "" , MB_OK);
        return false;
    }

    return true;
}

void DX11Cube::Update() {
    m_cube->Update();
}

void DX11Cube::Draw() {

    HRESULT hr;

    D3D11_BUFFER_DESC constantBufferDesc;
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
    constantBufferDesc.CPUAccessFlags = 0;
    constantBufferDesc.MiscFlags = 0;
    constantBufferDesc.StructureByteStride = 0;
    constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    ID3D11Buffer* cb;

    hr = m_pDevice->CreateBuffer(&constantBufferDesc , nullptr , &cb);

    if (FAILED(hr)) {
        MessageBox(nullptr , "CreateBuffer" , "" , MB_OK);
    }


    DirectX::XMMATRIX worldMatlix = DirectX::XMMatrixIdentity();
    DirectX::XMFLOAT4X4 mtx = m_cube->GetMatrix();
    worldMatlix = DirectX::XMLoadFloat4x4(&mtx);

    // 左手座標系で設定
    DirectX::XMMATRIX viewMatrix = GameCamera::GetInstance().GetViewMatrix();

    DirectX::XMMATRIX projectionMatrix =
        GameCamera::GetInstance().GetProjectionMatrix();

    ConstantBuffer constBuffer;

    DirectX::XMStoreFloat4x4(&constBuffer.m_world ,
        DirectX::XMMatrixTranspose(worldMatlix));
    DirectX::XMStoreFloat4x4(&constBuffer.m_view ,
        DirectX::XMMatrixTranspose(viewMatrix));
    DirectX::XMStoreFloat4x4(&constBuffer.m_projection ,
        DirectX::XMMatrixTranspose(projectionMatrix));

    m_pDeviceContext->UpdateSubresource(cb , 0 , nullptr , &constBuffer , 0 , 0);

    // 頂点バッファを描画で使えるようにセットする
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_pDeviceContext->IASetVertexBuffers(0 ,
        1 ,
        m_pVertexBuffer.GetAddressOf() ,
        &stride ,
        &offset);

    // プロミティブ・トポロジーをセット
    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // 定数バッファセット
    m_pDeviceContext->VSSetConstantBuffers(0 , 1 , &cb);

    m_pDeviceContext->VSSetShader(m_pVertexShader.Get() , 0 , 0);
    m_pDeviceContext->PSSetShader(m_pPixelShader.Get() , 0 , 0);
    m_pDeviceContext->IASetInputLayout(m_pInputLayout.Get());
    m_pDeviceContext->PSSetShaderResources(0 , 1 , m_pShaderResourceView.GetAddressOf());
    D3D11_VIEWPORT m_viewport = { 0.0f ,
        0.0f ,
        (float)SCREEN_WIDTH ,
        (float)SCREEN_HEIGHT ,
        0.0f ,
        1.0f };
    m_pDeviceContext->RSSetViewports(1 , &m_viewport);

    // 描画
    m_pDeviceContext->Draw(m_cube->GetCubevertex().size() , 0);
}

