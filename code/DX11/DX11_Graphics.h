//==============================================================================
/// Filename: DX11_Graphics.h
/// Description: DX11初期化
/// Copyright (C) Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

#include <d3d11.h>

#include "../Application/Win_Framework.h"
#include "../Game/Game_Device.h"
#include "DX11_Sampler.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

class DX11Graphics : public GameDevice {
public:
    //-----------------------------------------------------------------------------
    // public methods

    //-----------------------------------------------------------------------------
    /// 初期化
    /// 
    /// \param  [in]    _hWnd   ウィンドウハンドル
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    bool Init(
        /*[in]*/ HWND _hWnd);

    //-----------------------------------------------------------------------------
    /// 描画の前処理
    /// 
    /// \retrun void
    //-----------------------------------------------------------------------------
    void BeforeRender();


    //-----------------------------------------------------------------------------
    /// 描画の後処理
    /// 
    /// \retrun void
    //-----------------------------------------------------------------------------
    void AfterRender();

    //-----------------------------------------------------------------------------
    /// 解放処理
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Release();

    //-----------------------------------------------------------------------------
    /// デバイス取得
    /// 
    /// \return ID3D11Device*
    //-----------------------------------------------------------------------------
    ID3D11Device* GetDXDevice() const;

    //-----------------------------------------------------------------------------
    /// デバイスコンテキスト取得
    /// 
    /// \return GetDeviceContext*
    //-----------------------------------------------------------------------------
    ID3D11DeviceContext* GetDeviceContext() const;


    //-----------------------------------------------------------------------------
    /// スワップチェイン取得
    /// 
    /// \return IDXGISwapChain*
    //-----------------------------------------------------------------------------
    IDXGISwapChain* GetSwapChain() const;

    //-----------------------------------------------------------------------------
    /// レンダーターゲットビュー取得
    /// 
    /// \return ID3D11RenderTargetView*
    //-----------------------------------------------------------------------------
    ID3D11RenderTargetView* GetBackBufferPort() const;

    //-----------------------------------------------------------------------------
    /// ビューポート
    /// 
    /// \return ID3D11RenderTargetView*
    //-----------------------------------------------------------------------------
    D3D11_VIEWPORT GetViewPort() const;

    //-----------------------------------------------------------------------------

private:
    //-----------------------------------------------------------------------------
    // private variable

    ComPtr<ID3D11Device> m_pDevice;
    ComPtr<ID3D11DeviceContext> m_pDeviceContext;
    ComPtr<IDXGISwapChain> m_pSwapChain;
    ComPtr<ID3D11RenderTargetView> m_pBackBufferView;
    ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;
    ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
    ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
    D3D11_VIEWPORT m_viewport;

    ///<
    /// m_pDevice               デバイス
    /// m_pDeviceContext        デバイスコンテキスト
    /// m_pSwapChain            スワップチェイン
    /// m_pBackBufferView       レンダーターゲットビュー
    /// m_pDepthStencilBuffer    ステンシルバッファ
    /// m_pDepthStencilState    ステンシルステート
    /// m_pDepthStencilView     ステンシルビュー
    /// m_viewport              ビューポート
    ///

    //-----------------------------------------------------------------------------
};
