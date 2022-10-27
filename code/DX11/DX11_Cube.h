//==============================================================================
/// Filename: DX11_Cube.h
/// Description: ボックス表示するクラス
/// Copyright (C) Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

#include "../../framework.h"

#include "../Game/Game_Camera.h"

#include "../Game/Game_Cube.h"

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

class DX11Cube :public GameCube {
public:
    //-----------------------------------------------------------------------------
    // public methods

    //-----------------------------------------------------------------------------
    /// 初期化
    /// 
    /// \param [in] _pDevice 表示するAPIのデバイスのポインタ
    /// 
    /// \return bool
    //-----------------------------------------------------------------------------
    bool Init(
        /*[in]*/ GameDevice* _pDevice);

    //-----------------------------------------------------------------------------
    /// 更新処理
    /// 
    /// \param [in] _pDevice 表示するAPIのデバイスのポインタ
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Update(
        /*[in]*/ GameDevice* _pDevice);

    //-----------------------------------------------------------------------------
    /// 描画
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Draw();

    //-----------------------------------------------------------------------------
    /// 解放処理
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Release();

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

    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pDeviceContext = nullptr;
    //-----------------------------------------------------------------------------

    ///<
    /// m_pVertexShader 頂点シェーダー
    /// m_pPixelShader ピクセルシェーダー
    /// m_pInputLayout 入力レイアウト
    /// m_pVertexBuffer 頂点バッファ
    /// m_pShaderResourceView シェーダーリソースビュー
    /// 
    /// m_pDevice　DirectX11のデバイス
    /// m_pDeviceContext DirectX11のデバイスコンテキスト
    ///<

    //-----------------------------------------------------------------------------
};
