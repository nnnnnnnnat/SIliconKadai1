#include <vector>
#include <random>

#include <DirectXMath.h>

#include "DX11_Polygon.h"
#include "DX11_Graphics.h"
#include "DX11_Shader.h"
#include "DX11_Settransform.h"

bool DX11Polygon::Initialize(
    const float _width ,
    const float _height ,
    DirectX::XMFLOAT4 _color) {

    m_vertex[0] = { { -_width , +_height , 0.0f } , { 0.0f , 0.0f } , _color };
    m_vertex[1] = { { +_width , +_height , 0.0f } , { 0.0f , 1.0f } , _color };
    m_vertex[2] = { { -_width , -_height , 0.0f } , { 1.0f , 0.0f } , _color };
    m_vertex[3] = { { +_width , -_height , 0.0f } , { 1.0f , 1.0f } , _color };

    ID3D11Device* dev;
    ID3D11DeviceContext* devcontext;
    HRESULT hr;

    dev = DX11Graphics::GetInstance().GetDXDevice();
    devcontext = DX11Graphics::GetInstance().GetDeviceContext();

    //-----------------------------
    // 頂点バッファ作成
    // ・上記で作った３つの頂点はそのままでは描画に使用できないんす…
    // ・ビデオメモリ側に「頂点バッファ」という形で作る必要があります！
    // ・今回は効率無視して、その場で作って、使って、すぐ捨てます。
    //-----------------------------
    // 作成するバッファの仕様を決める
    // ・今回は頂点バッファにするぞ！って感じの設定
    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// デバイスにバインドするときの種類(頂点バッファ、インデックスバッファ、定数バッファなど)
    vbDesc.ByteWidth = sizeof(m_vertex);					// 作成するバッファのバイトサイズ
    vbDesc.MiscFlags = 0;							// その他のフラグ
    vbDesc.StructureByteStride = 0;					// 構造化バッファの場合、その構造体のサイズ
    vbDesc.Usage = D3D11_USAGE_DEFAULT;				// 作成するバッファの使用法
    vbDesc.CPUAccessFlags = 0;

    // 上の仕様を渡して頂点バッファを作ってもらう
    // もちろんデバイスさんにお願いする
    D3D11_SUBRESOURCE_DATA initData = { &m_vertex[0] , sizeof(m_vertex) , 0 };	// 書き込むデータ
    // 頂点バッファの作成
    dev->CreateBuffer(&vbDesc , &initData , &mp_vertexBuffer);

    //=====================================================
    // シェーダーの作成
    //=====================================================
    // 頂点シェーダーを読み込み＆コンパイル
    ComPtr<ID3DBlob> compiledVS;
    if (FAILED(D3DCompileFromFile(VS_FILENAME , nullptr , nullptr , "VS" , "vs_5_0" , 0 , 0 , &compiledVS , nullptr))) {
        MessageBox(nullptr , "vs" , "" , MB_OK);
        return false;
    }
    // ピクセルシェーダーを読み込み＆コンパイル
    ComPtr<ID3DBlob> compiledPS;
    if (FAILED(D3DCompileFromFile(PS_FILENAME , nullptr , nullptr , "PS" , "ps_5_0" , 0 , 0 , &compiledPS , nullptr))) {
        MessageBox(nullptr , "ps" , "" , MB_OK);
        return false;
    }

    // 頂点シェーダー作成
    if (FAILED(dev->CreateVertexShader(compiledVS->GetBufferPointer() ,
        compiledVS->GetBufferSize() ,
        nullptr ,
        &mp_vertexShader))) {
        MessageBox(nullptr , "vx create" , "" , MB_OK);
        return false;
    }
    // ピクセルシェーダー作成
    if (FAILED(dev->CreatePixelShader(compiledPS->GetBufferPointer() ,
        compiledPS->GetBufferSize() ,
        nullptr ,
        &mp_pixelShader))) {
        MessageBox(nullptr , "ps create" , "" , MB_OK);
        return false;
    }

    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION" , 0 , DXGI_FORMAT_R32G32B32_FLOAT , 0 , D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 } ,
        { "TEXCOORD" , 0 , DXGI_FORMAT_R32G32_FLOAT , 0 , D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 } ,
        { "COLOR" , 0 , DXGI_FORMAT_R32G32B32A32_FLOAT , 0 , D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA , 0 }
    };

    hr = dev->CreateInputLayout(
        layout ,
        ARRAYSIZE(layout) ,
        compiledVS->GetBufferPointer() ,
        compiledVS->GetBufferSize() ,
        &mp_inputLayout);

    // 頂点インプットレイアウト作成
    if (FAILED(hr)) {
        MessageBox(nullptr , "input" , "" , MB_OK);
        return false;
    }

    //--------------------------★変更↓--------------------------
    const int iPixSize = 32;//縦横ピクセル数

    //テクスチャの作成
    ComPtr<ID3D11Texture2D> D3DTexture;
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
    if (FAILED(hr)) {
        MessageBox(nullptr , "CreateTexture2D" , "" , MB_OK);
        return false;
    }

    //テクスチャ書き替え
    D3D11_MAPPED_SUBRESOURCE msr;
    devcontext->Map(D3DTexture.Get() , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &msr);

    //乱数生成
    std::random_device seed;
    std::mt19937 random(seed());
    std::uniform_int_distribution<> number(0 , 255);//0～255までのランダムな数

    byte srcData[iPixSize * iPixSize * 4] = { 0 };//ビットマップを黒で初期化
    for (int i = 0; i < iPixSize * iPixSize * 4; i += 4) {
        srcData[i] = number(random);//Red
        srcData[i + 1] = number(random);//Green
        srcData[i + 2] = number(random);//Blue
    }
    memcpy(msr.pData , srcData , sizeof(srcData));
    //--------------------------★変更↑--------------------------
    devcontext->Unmap(D3DTexture.Get() , 0);

    //シェーダリソースビューの作成
    D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
    srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srv.Texture2D.MipLevels = 1;
    hr = dev->CreateShaderResourceView(D3DTexture.Get() , &srv , &mp_srv);
    if (FAILED(hr)) {
        MessageBox(nullptr , "CreateShaderResourceView" , "" , MB_OK);
        return false;
    }
    return true;
}

void DX11Polygon::Draw() {

    ID3D11DeviceContext* p_devContext = DX11Graphics::GetInstance().GetDeviceContext();

    // 頂点バッファを描画で使えるようにセットする
    UINT stride = sizeof(VerteAx);
    UINT offset = 0;
    p_devContext->IASetVertexBuffers(0 , 1 , mp_vertexBuffer.GetAddressOf() , &stride , &offset);
    // プロミティブ・トポロジーをセット
    p_devContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    //-----------------------------
    // シェーダーをセット
    //-----------------------------
    p_devContext->VSSetShader(mp_vertexShader.Get() , 0 , 0);
    p_devContext->PSSetShader(mp_pixelShader.Get() , 0 , 0);
    p_devContext->IASetInputLayout(mp_inputLayout.Get());
    p_devContext->PSSetShaderResources(0 , 1 , mp_srv.GetAddressOf());

    // こんな感じで、ひたすらデバイスコンテキストに情報を渡す

    //-----------------------------
    // 描画実行
    //-----------------------------
    // デバイスコンテキストくん、上記のセットした内容で描画してください、とお願いする
    p_devContext->Draw(4 , 0);

}
