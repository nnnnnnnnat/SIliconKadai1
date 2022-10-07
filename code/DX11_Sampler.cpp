//==============================================================================
/// Filename: DX11_Sampler.h
/// Description: �T���v���[����
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#include "DX11_Sampler.h"

DX11Sampler* DX11Sampler::GetInstance() {
    static DX11Sampler instance;
    return &instance;
}

void DX11Sampler::Init() {

    // �ϐ��錾
    ID3D11Device* device;

    // �f�o�C�X���擾����
    device = DX11Graphics::GetInstance().GetDXDevice();

    for (int i = 0; i < 5; i++) {
        // �ϐ��錾���ă[���������[����
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

        // �T���v���[�X�e�[�g����
        HRESULT hr = device->CreateSamplerState(&smpDesc , &m_samplerState[i]);
        if (FAILED(hr)) {
            MessageBox(nullptr , "CreateSamplerState" , "" , MB_OK);
        }
    }
}

void DX11Sampler::Set(const Sampler_Mode _mode) {

    // �f�o�C�X�R���e�L�X�g
    ID3D11DeviceContext* devicecontext;

    // �f�o�C�X�R���e�L�X�g���擾����
    devicecontext = DX11Graphics::GetInstance().GetDeviceContext();

    // �T���v���[���[�h��ύX����
    devicecontext->PSSetSamplers(0 , 1 , &m_samplerState[(int)_mode]);
    HWND hwnd = GetActiveWindow();

}
