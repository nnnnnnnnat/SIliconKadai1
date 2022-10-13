//==============================================================================
/// Filename: DX12_Graphics.h
/// Description: DX12
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================


#pragma once

#include "../../framework.h"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

#include <DirectXMath.h>

#pragma comment(lib , "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "DX12_util.h"

class DX12Graphics : private NonCopyable {
public:
    //-----------------------------------------------------------------------------
    // public methods

    //-----------------------------------------------------------------------------
    /// インスタンス受け取り
    /// 
    /// \return DX12Graphics&
    //-----------------------------------------------------------------------------
    static DX12Graphics& GetInstance();

    //-----------------------------------------------------------------------------
    /// DX12初期化
    /// 
    /// \param [in] _hWnd ウィンドウハンドル
    /// \param [in] _width ウィンドウの横の大きさ
    /// \param [in] _height ウィンドウの縦の大きさ
    /// 
    /// \return bool
    //-----------------------------------------------------------------------------
    bool Init(HWND _hWnd , const UINT _width , const UINT _height);

    //-----------------------------------------------------------------------------
    /// 更新
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Update();

    //-----------------------------------------------------------------------------
    /// 描画前処理
    /// 
    /// \return void
    //----------------------------------------------------------------------
    void BeforeRender();

    //-----------------------------------------------------------------------------
    /// 描画後処理
    /// 
    /// \return void
    //----------------------------------------------------------------------
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
    /// \return ID3D12Device*
    //-----------------------------------------------------------------------------
    ID3D12Device* GetDevice();

    //-----------------------------------------------------------------------------
    /// コマンドリスト取得
    /// 
    /// \return ID3D12CommandList*
    //-----------------------------------------------------------------------------
    ID3D12GraphicsCommandList* GetCommandList();

    //-----------------------------------------------------------------------------
    /// バックバッファの番号を取得
    /// 
    /// \return int
    //-----------------------------------------------------------------------------
    int GetSwapChainCurrentBackBufferindex();

    //-----------------------------------------------------------------------------
    /// ビューポート取得
    /// 
    /// \return D3D12_VIEWPORT
    //-----------------------------------------------------------------------------
    D3D12_VIEWPORT GetViewPort();

    //-----------------------------------------------------------------------------
    /// シザー短形取得
    /// 
    /// \return D3D12_RECT
    //-----------------------------------------------------------------------------
    D3D12_RECT GetRect();

    //-----------------------------------------------------------------------------
    /// ウィンドウの横の長さ取得
    /// 
    /// \return D3D12_RECT
    //-----------------------------------------------------------------------------
    int GetWindowWidth();

    //-----------------------------------------------------------------------------
    /// ウィンドウの縦の長さ取得
    /// 
    /// \return D3D12_RECT
    //-----------------------------------------------------------------------------
    int GetWindowHeight();

    //-----------------------------------------------------------------------------

private:
    //-----------------------------------------------------------------------------
    // private method

    //-----------------------------------------------------------------------------
    /// 描画終了待ち関数
    /// 
    /// \param [out] _pCommandQueue コマンドキュー
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void WaitForCommandQueue(
        ID3D12CommandQueue* _pCommandQueue);

    //-----------------------------------------------------------------------------
    /// 背景色設定
    /// 
    /// \param [in] _color[4] 背景色
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void SetBackGroundColor(
        const float _color[4]);

    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // private variable

    ComPtr<ID3D12Device> m_pDevice;
    ComPtr<IDXGISwapChain3> m_pSwapChain;
    ComPtr<IDXGIFactory3> m_pFactory;
    ComPtr<ID3D12CommandAllocator> m_pCommandAllocator;
    ComPtr<ID3D12CommandQueue> m_pCommandQueue;
    ComPtr<ID3D12Fence>  m_pQueueFence;
    ComPtr<ID3D12DescriptorHeap> m_pDescriptorHeapRTV;
    ComPtr<ID3D12Resource> m_pRenderTarget[2];
    ComPtr<ID3D12Resource> m_pRenderTargetDepth;
    ComPtr<ID3D12GraphicsCommandList> m_pCommandList;

    ComPtr<ID3D12DescriptorHeap> m_pDescriptorHeapCB;
    ComPtr<ID3D12DescriptorHeap> m_pDescriptorHeapDSB;

    D3D12_CPU_DESCRIPTOR_HANDLE m_rtvHandle[2];
    D3D12_CPU_DESCRIPTOR_HANDLE m_handleDSV;
    HANDLE m_queueEvent;
    D3D12_VIEWPORT m_viewPort;
    D3D12_RECT m_rect;
    int m_windowWidth = 0;
    int m_windowHeight = 0;
    float m_backGroundColor[4] = { 0.1f , 0.5f , 0.5f , 1.0f };

    ///<
    /// m_pDevice DX12デバイス
    /// m_pSwapChain スワップチェイン
    /// m_pFactory ファクトリー
    /// m_pCommandAllocator コマンドアロケータ―
    /// m_pCommandQueue コマンドキュー
    /// m_pQueueFence フェンス
    /// m_pDescriptorHeap ディスクリプタヒープ
    /// m_pCommandList グラフィックコマンドリスト
    ///<

    //-----------------------------------------------------------------------------

};
