//==============================================================================
/// Filename: DX11_Cube.cpp
/// Description: ボックス表示するクラス
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#include "DX11_Cube.h"
#include "DX11_Graphics.h"

bool DX11Cube::Init(const float _width , const float _height , const float _depth) {

    HRESULT hr;
    ID3D11Device* dev;
    ID3D11DeviceContext* devcontext;

    // デバイスとデバイスコンテキスト取得
    dev = DX11Graphics::GetInstance().GetDXDevice();
    devcontext = DX11Graphics::GetInstance().GetDeviceContext();

    DirectX::XMFLOAT4 color = { 1.0f , 1.0f , 1.0f , 1.0f };

    // uv座標格納
    std::array<DirectX::XMFLOAT2 , 4> uc;
    uc[0] = { 0.0f , 0.0f };
    uc[1] = { 0.0f , 1.0f };
    uc[2] = { 1.0f , 0.0f };
    uc[3] = { 1.0f , 1.0f };

    // 頂点に情報を格納していく
    m_cube[0] = { { -_width , +_height , -_depth } , color , uc[0] };
    m_cube[1] = { { +_width , +_height , -_depth } , color , uc[2] };
    m_cube[2] = { { +_width , -_height , -_depth } , color , uc[3] };
    m_cube[3] = { { +_width , -_height , -_depth } , color , uc[3] };
    m_cube[4] = { { -_width , -_height , -_depth } , color , uc[1] };
    m_cube[5] = { { -_width , +_height , -_depth } , color , uc[0] };

    m_cube[6] = { { +_width , -_height , +_depth } , color , uc[0] };
    m_cube[7] = { { +_width , +_height , +_depth } , color , uc[1] };
    m_cube[8] = { { -_width , +_height , +_depth } , color , uc[2] };
    m_cube[9] = { { -_width , +_height , +_depth } , color , uc[2] };
    m_cube[10] = { { -_width , -_height , +_depth } , color , uc[3] };
    m_cube[11] = { { +_width , -_height , +_depth } , color , uc[0] };

    m_cube[12] = { { +_width , +_height , -_depth } , color , uc[0] };
    m_cube[13] = { { +_width , +_height , +_depth } , color , uc[1] };
    m_cube[14] = { { +_width , -_height , +_depth } , color , uc[2] };
    m_cube[15] = { { +_width , -_height , +_depth } , color , uc[2] };
    m_cube[16] = { { +_width , -_height , -_depth } , color , uc[3] };
    m_cube[17] = { { +_width , +_height , -_depth } , color , uc[0] };

    m_cube[18] = { { -_width , -_height , +_depth } , color , uc[0] };
    m_cube[19] = { { -_width , +_height , +_depth } , color , uc[3] };
    m_cube[20] = { { -_width , +_height , -_depth } , color , uc[2] };
    m_cube[21] = { { -_width , +_height , -_depth } , color , uc[2] };
    m_cube[22] = { { -_width , -_height , -_depth } , color , uc[1] };
    m_cube[23] = { { -_width , -_height , +_depth } , color , uc[0] };

    m_cube[24] = { { -_width , +_height , +_depth } , color , uc[0] };
    m_cube[25] = { { +_width , +_height , +_depth } , color , uc[1] };
    m_cube[26] = { { +_width , +_height , -_depth } , color , uc[2] };
    m_cube[27] = { { +_width , +_height , -_depth } , color , uc[2] };
    m_cube[28] = { { -_width , +_height , -_depth } , color , uc[3] };
    m_cube[29] = { { -_width , +_height , +_depth } , color , uc[0] };

    m_cube[30] = { { +_width , -_height , -_depth } , color , uc[0] };
    m_cube[31] = { { +_width , -_height , +_depth } , color , uc[1] };
    m_cube[32] = { { -_width , -_height , +_depth } , color , uc[2] };
    m_cube[33] = { { -_width , -_height , +_depth } , color , uc[2] };
    m_cube[34] = { { -_width , -_height , -_depth } , color , uc[3] };
    m_cube[35] = { { +_width , -_height , -_depth } , color , uc[0] };

    //-----------------------------
    // 頂点バッファ作成
    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // デバイスにバインドするときの種類(頂点バッファ、インデックスバッファ、定数バッファなど)
    vbDesc.ByteWidth = sizeof(m_cube); // 作成するバッファのバイトサイズ
    vbDesc.MiscFlags = 0; // その他のフラグ
    vbDesc.StructureByteStride = 0;	// 構造化バッファの場合、その構造体のサイズ
    vbDesc.Usage = D3D11_USAGE_DEFAULT;	// 作成するバッファの使用法
    vbDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initData = { &m_cube[0] , sizeof(m_cube) , 0 }; // 書き込むデータ
    // 頂点バッファの作成
    hr = dev->CreateBuffer(&vbDesc , &initData , &m_pVertexBuffer);
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
    hr = dev->CreateVertexShader(compiledVS->GetBufferPointer() ,
        compiledVS->GetBufferSize() ,
        nullptr ,
        &m_pVertexShader);
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "vx create" , "" , MB_OK);
        return false;
    }

    // ピクセルシェーダー作成
    hr = dev->CreatePixelShader(compiledPS->GetBufferPointer() ,
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

    hr = dev->CreateInputLayout(
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

    hr = dev->CreateTexture2D(&td , nullptr , &D3DTexture);
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "CreateTexture2D" , "" , MB_OK);
        return false;
    }

    //テクスチャ書き替え
    D3D11_MAPPED_SUBRESOURCE msr;
    devcontext->Map(D3DTexture.Get() , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &msr);

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

    devcontext->Unmap(D3DTexture.Get() , 0);

    //シェーダリソースビューの作成
    D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
    srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srv.Texture2D.MipLevels = 1;
    hr = dev->CreateShaderResourceView(D3DTexture.Get() , &srv , &m_pShaderResourceView);
    if (FAILED(hr)) {
        MessageBox(nullptr , "CreateShaderResourceView" , "" , MB_OK);
        return false;
    }

    return true;
}

void DX11Cube::Draw() {

    HRESULT hr;
    ID3D11Device* dev;
    ID3D11DeviceContext* devcontext;

    dev = DX11Graphics::GetInstance().GetDXDevice();
    devcontext = DX11Graphics::GetInstance().GetDeviceContext();

    D3D11_BUFFER_DESC constantBufferDesc;
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
    constantBufferDesc.CPUAccessFlags = 0;
    constantBufferDesc.MiscFlags = 0;
    constantBufferDesc.StructureByteStride = 0;
    constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    ID3D11Buffer* cb;

    hr = dev->CreateBuffer(&constantBufferDesc , nullptr , &cb);

    if (FAILED(hr)) {
        MessageBox(nullptr , "CreateBuffer" , "" , MB_OK);
    }

    // カメラの座標を回転させる
    static float s_angle = 0.0f;
    s_angle += 1.0f;
    if (s_angle >= 360.0f) {
        s_angle -= 360.0f;
    }
    DirectX::XMMATRIX worldMatlix = DirectX::XMMatrixTranslation(0.0f , 0.0f , 0.0f);
    DirectX::XMVECTOR eye = { sinf(DirectX::XMConvertToRadians(45.0f)) * 2.0f ,
        2.0f ,
        cosf(DirectX::XMConvertToRadians(45.0f)) * 2.0f , 0.0f };
    DirectX::XMVECTOR focus = { 0.0f , 0.0f , 0.0f , 0.0f };
    DirectX::XMVECTOR up = { 0.0f , 1.0f , 0.0f , 0.0f };

    // 左手座標系で設定
    DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(eye , focus , up);

    // カメラ(仮)を設定
    constexpr float fov = DirectX::XMConvertToRadians(45.0f);
    float aspect = 1280.0f / 720.0f;
    float nearz = 0.1f;
    float farz = 100.0f;

    DirectX::XMMATRIX projectionMatrix =
        DirectX::XMMatrixPerspectiveFovLH(fov , aspect , nearz , farz);

    ConstantBuffer constBuffer;

    DirectX::XMStoreFloat4x4(&constBuffer.m_world ,
        DirectX::XMMatrixTranspose(worldMatlix));
    DirectX::XMStoreFloat4x4(&constBuffer.m_view ,
        DirectX::XMMatrixTranspose(viewMatrix));
    DirectX::XMStoreFloat4x4(&constBuffer.m_projection ,
        DirectX::XMMatrixTranspose(projectionMatrix));

    devcontext->UpdateSubresource(cb , 0 , nullptr , &constBuffer , 0 , 0);

    // 頂点バッファを描画で使えるようにセットする
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    devcontext->IASetVertexBuffers(0 ,
        1 ,
        m_pVertexBuffer.GetAddressOf() ,
        &stride ,
        &offset);

    // プロミティブ・トポロジーをセット
    devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // 定数バッファセット
    devcontext->VSSetConstantBuffers(0 , 1 , &cb);

    devcontext->VSSetShader(m_pVertexShader.Get() , 0 , 0);
    devcontext->PSSetShader(m_pPixelShader.Get() , 0 , 0);
    devcontext->IASetInputLayout(m_pInputLayout.Get());
    devcontext->PSSetShaderResources(0 , 1 , m_pShaderResourceView.GetAddressOf());
    D3D11_VIEWPORT m_viewport = DX11Graphics::GetInstance().GetViewPort();
    devcontext->RSSetViewports(1 , &m_viewport);

    // 描画
    devcontext->Draw(m_cube.size() , 0);
}

