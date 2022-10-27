//==============================================================================
/// Filename: DX11_Sampler.h
/// Description: �T���v���[����
/// Copyright (C) Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#include "DX11_Sampler.h"

DX11Sampler* DX11Sampler::GetInstance() {
    static DX11Sampler instance;
    return &instance;
}

void DX11Sampler::Init(ID3D11Device* _pDev) {

    for (int i = 0; i < 5; i++) { // for
        // �ϐ��錾���ă[���������[����
        D3D11_SAMPLER_DESC smpDesc;
        ZeroMemory(&smpDesc , sizeof(smpDesc));

        if (i % 2) { // if
            smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        }
        else { // else
            smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        }
        smpDesc.AddressU = m_rappingMode[i];
        smpDesc.AddressV = m_rappingMode[i];
        smpDesc.AddressW = m_rappingMode[i];

        // �T���v���[�X�e�[�g����
        HRESULT hr = _pDev->CreateSamplerState(&smpDesc , &m_samplerState[i]);
        if (FAILED(hr)) { // if
            MessageBoxA(nullptr , "CreateSamplerState" , "" , MB_OK);
        }
    }
}

void DX11Sampler::Set(ID3D11DeviceContext* _pDevContext , const Sampler_Mode _mode) {

    // �T���v���[���[�h��ύX����
    _pDevContext->PSSetSamplers(0 , 1 , &m_samplerState[(int)_mode]);
    HWND hwnd = GetActiveWindow();

}
