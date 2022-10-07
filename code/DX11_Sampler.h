//==============================================================================
/// Filename: DX11_Sampler.h
/// Description: �T���v���[����
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

#include "DX11_Graphics.h"
#include "Utilities.h"

class DX11Sampler : private Util::NonCopyable {
public:
    //-----------------------------------------------------------------------------
    // public methods
    enum class Sampler_Mode {
        WRAP ,
        CLAMP ,
        MIRROR ,
        BORDER ,
        MIRROR_ONCE ,
        max ,
    };

    //-----------------------------------------------------------------------------
    /// �C���X�^���X�󂯎��
    /// 
    /// \return DX11Sampler*
    //-----------------------------------------------------------------------------
    static DX11Sampler* GetInstance();

    //-----------------------------------------------------------------------------
    /// ������
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Init();

    //-----------------------------------------------------------------------------
    /// �T���v���[���[�h�i�[
    /// 
    /// \param[in] _mode �T���v���[���[�h�̎w��
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Set(
        /*[in]*/ const Sampler_Mode _mode);

    //-----------------------------------------------------------------------------

private:
    //-----------------------------------------------------------------------------
    // private methods

    //-----------------------------------------------------------------------------
    D3D11_TEXTURE_ADDRESS_MODE  m_rappingMode[5] = {
        D3D11_TEXTURE_ADDRESS_WRAP ,
        D3D11_TEXTURE_ADDRESS_CLAMP ,
        D3D11_TEXTURE_ADDRESS_MIRROR ,
        D3D11_TEXTURE_ADDRESS_BORDER ,
        D3D11_TEXTURE_ADDRESS_MIRROR_ONCE
    };

    ID3D11SamplerState* m_samplerState[5] = { nullptr , nullptr , nullptr , nullptr , nullptr };

    const char* m_rappingModeName[5] = {
        "D3D11_TEXTURE_ADDRESS_WRAP" ,
        "D3D11_TEXTURE_ADDRESS_CLAMP" ,
        "D3D11_TEXTURE_ADDRESS_MIRROR" ,
        "D3D11_TEXTURE_ADDRESS_BORDER" ,
        "D3D11_TEXTURE_ADDRESS_MIRROR_ONCE"
    };
    //-----------------------------------------------------------------------------
    ///<
    /// m_rappingMode[5] �T���v���[���[�h
    /// m_samplerState[5] �T���v���[�X�e�[�g�i�[�p�ϐ�
    /// m_rappingModeName[5] �T���v���[���[�h�̖��O �i�[�p�ϐ�
    ///<

    //-----------------------------------------------------------------------------
};
