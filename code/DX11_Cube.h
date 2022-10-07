//==============================================================================
/// Filename: DX11_Cube.h
/// Description: ボックス表示するクラス
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

#include "../framework.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <array>
#include <wrl/client.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

using Microsoft::WRL::ComPtr;

typedef std::array<Vertex , 36> VertexCube;

class DX11Cube {
public:
    //-----------------------------------------------------------------------------
    // public methods

    //-----------------------------------------------------------------------------
    /// 初期化
    /// 
    /// \param [in] _width 横の大きさ
    /// \param [in] _height 縦の大きさ
    /// \param [in] _depth 奥行き
    /// 
    /// \return bool
    //-----------------------------------------------------------------------------
    bool Init(
        /*[in]*/ const float _width ,
        /*[in]*/ const float _height ,
        /*[in]*/ const float _depth);

    //-----------------------------------------------------------------------------
    /// 描画
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Draw();

    //-----------------------------------------------------------------------------

private:

    //-----------------------------------------------------------------------------
    // private methods

    //-----------------------------------------------------------------------------
    ComPtr<ID3D11VertexShader>	m_pVertexShader = nullptr;
    ComPtr<ID3D11PixelShader>	m_pPixelShader = nullptr;
    ComPtr<ID3D11InputLayout>	m_pInputLayout = nullptr;
    ComPtr<ID3D11Buffer> m_pVertexBuffer = nullptr;
    ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView = nullptr;
    VertexCube m_cube;
    //-----------------------------------------------------------------------------

    ///<
    /// m_pVertexShader 頂点シェーダー
    /// m_pPixelShader ピクセルシェーダー
    /// m_pInputLayout 入力レイアウト
    /// m_pVertexBuffer 頂点バッファ
    /// m_pShaderResourceView シェーダーリソースビュー
    /// m_cube キューブ表示をする変数
    ///<

    //-----------------------------------------------------------------------------
};
