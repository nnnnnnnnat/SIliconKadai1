//==============================================================================
/// Filename: DX11_Cube.h
/// Description: ボックス表示するクラス
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
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

class DX11Cube {
public:
    //-----------------------------------------------------------------------------
    // public methods

    //-----------------------------------------------------------------------------
    /// 初期化
    /// 
    /// \param [in] _pDev DX11のデバイス
    /// \param [in] _pDevContext DX11のデバイスコンテキスト
    /// \param [in] _cube 描画するキューブのポインタ
    /// 
    /// \return bool
    //-----------------------------------------------------------------------------
    bool Init(
        ID3D11Device* _pDev ,
        ID3D11DeviceContext* _pDevContext ,
        GameCube* _cube);

    //-----------------------------------------------------------------------------
    /// 更新処理
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Update();

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

    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pDeviceContext = nullptr;

    GameCube* m_cube;
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
    /// 
    /// m_cube キューブ表示をする変数
    ///<

    //-----------------------------------------------------------------------------
};
