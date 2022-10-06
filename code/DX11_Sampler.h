#pragma once

#include "DX11_Graphics.h"
#include <stdio.h>

// GameInit �ɋL��
// C_Sampler::GetInstance()->Init();
// C_Sampler::GetInstance()->Set(C_Sampler::Sampler_Mode::WRAP);

class DX11Sampler {
public:
    DX11Sampler(const DX11Sampler&) = delete;
    DX11Sampler& operator=(const DX11Sampler&) = delete;
    DX11Sampler(DX11Sampler&&) = delete;
    DX11Sampler& operator=(DX11Sampler&&) = delete;
    DX11Sampler() {}

    enum class Sampler_Mode {
        WRAP ,
        CLAMP ,
        MIRROR ,
        BORDER ,
        MIRROR_ONCE ,
        max ,
    };

    static DX11Sampler* GetInstance() {
        static DX11Sampler instance;
        return &instance;
    }

    void Init() {

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
                printf("sampler error");
            }
        }
    }

    void Set(Sampler_Mode _mode) {

        // �f�o�C�X�R���e�L�X�g
        ID3D11DeviceContext* devicecontext;

        // �f�o�C�X�R���e�L�X�g���擾����
        devicecontext = DX11Graphics::GetInstance().GetDeviceContext();

        // �T���v���[���[�h��ύX����
        devicecontext->PSSetSamplers(0 , 1 , &m_samplerState[(int)_mode]);
        HWND hwnd = GetActiveWindow();
        // SetWindowText(hwnd , m_rappingModeName[(int)_mode]);
    }

private:

    // �T���v���[���[�h
    D3D11_TEXTURE_ADDRESS_MODE  m_rappingMode[5] = {
        D3D11_TEXTURE_ADDRESS_WRAP ,
        D3D11_TEXTURE_ADDRESS_CLAMP ,
        D3D11_TEXTURE_ADDRESS_MIRROR ,
        D3D11_TEXTURE_ADDRESS_BORDER ,
        D3D11_TEXTURE_ADDRESS_MIRROR_ONCE
    };

    // �T���v���[�X�e�[�g
    ID3D11SamplerState* m_samplerState[5] = { nullptr , nullptr , nullptr , nullptr , nullptr };

    // ���O
    const char* m_rappingModeName[5] = {
        "D3D11_TEXTURE_ADDRESS_WRAP" ,
        "D3D11_TEXTURE_ADDRESS_CLAMP" ,
        "D3D11_TEXTURE_ADDRESS_MIRROR" ,
        "D3D11_TEXTURE_ADDRESS_BORDER" ,
        "D3D11_TEXTURE_ADDRESS_MIRROR_ONCE"
    };
};
