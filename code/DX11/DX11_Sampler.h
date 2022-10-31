//==============================================================================
/// Filename: DX11_Sampler.h
/// Description: DirectX11 サンプラー処理
/// Copyright (C) Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

#include "DX11_Graphics.h"
#include "../Application/Win_Framework.h"

class DX11Sampler : private NonCopyable {
public:

    enum class Sampler_Mode {
        WRAP ,
        CLAMP ,
        MIRROR ,
        BORDER ,
        MIRROR_ONCE ,
        max ,
    };

    //-----------------------------------------------------------------------------
    // public methods

    //-----------------------------------------------------------------------------
    /// インスタンス受け取り
    /// 
    /// \return DX11Sampler*
    //-----------------------------------------------------------------------------
    static DX11Sampler* GetInstance();

    //-----------------------------------------------------------------------------
    /// 初期化
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Init(
        /*[in]*/ ID3D11Device* _pDev);

    //-----------------------------------------------------------------------------
    /// サンプラーモード格納
    /// 
    /// \param  [in]    _mode   サンプラーモードの指定
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Set(
        /*[in]*/ ID3D11DeviceContext* _pDevContext ,
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
    /// m_rappingMode[5]        サンプラーモード
    /// m_samplerState[5]       サンプラーステート格納用変数
    /// m_rappingModeName[5]    サンプラーモードの名前 格納用変数
    ///<

    //-----------------------------------------------------------------------------
};
