//==============================================================================
/// Filename: DX11_Sampler.h
/// Description: サンプラー処理
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#include "DX11_Sampler.h"

DX11Sampler* DX11Sampler::GetInstance() {
    static DX11Sampler instance;
    return &instance;
}

void DX11Sampler::Init() {

    // 変数宣言
    ID3D11Device* device;

    // デバイスを取得する
    device = DX11Graphics::GetInstance().GetDXDevice();

    for (int i = 0; i < 5; i++) {
        // 変数宣言してゼロメモリーする
        D3D11_SAMPLER_DESC smpDesc;
        ZeroMemory(&smpDesc , sizeof(smpDesc));

        if (i % 2) {
            smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        }
        else {
            smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        }
        smpDesc.AddressU = m_rappingMode[i];
        smpDesc.AddressV = m_rappingMode[i];
        smpDesc.AddressW = m_rappingMode[i];

        // サンプラーステート生成
        HRESULT hr = device->CreateSamplerState(&smpDesc , &m_samplerState[i]);
        if (FAILED(hr)) {
            MessageBox(nullptr , "CreateSamplerState" , "" , MB_OK);
        }
    }
}

void DX11Sampler::Set(const Sampler_Mode _mode) {

    // デバイスコンテキスト
    ID3D11DeviceContext* devicecontext;

    // デバイスコンテキストを取得する
    devicecontext = DX11Graphics::GetInstance().GetDeviceContext();

    // サンプラーモードを変更する
    devicecontext->PSSetSamplers(0 , 1 , &m_samplerState[(int)_mode]);
    HWND hwnd = GetActiveWindow();

}
