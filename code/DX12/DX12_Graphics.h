//==============================================================================
/// Filename: DX12_Graphics.h
/// Description: DX12
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================


#pragma once

#include "../../framework.h"

#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>

#include <DirectXMath.h>

#pragma comment(lib , "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

class DX12Graphics : private Util::NonCopyable {
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
    /// \return ID3D12Device
    //-----------------------------------------------------------------------------
    ID3D12Device* GetDevice();

    //-----------------------------------------------------------------------------

private:
    //-----------------------------------------------------------------------------
    // private method

    //-----------------------------------------------------------------------------
    /// リソースバリア設定
    /// 
    /// \param [out] _pRommandList コマンドリスト
    /// \param [in] _pResource レンダーターゲット
    /// \param [in] _before リソースステートの同期のための引数
    /// \param [in] _after リソースステートの同期のための引数
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void SetResourceBarrier(
        ID3D12GraphicsCommandList* _pRommandList ,
        ID3D12Resource* _pResource ,
        D3D12_RESOURCE_STATES _before ,
        D3D12_RESOURCE_STATES _after);

    void WaitForCommandQueue(
        ID3D12CommandQueue* _pCommandQueue);

    void SetBackGroundColor(
        float _color[4]);

    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // private variable

    ComPtr<ID3D12Device> m_pDevice;
    ComPtr<ID3D12CommandQueue> m_pCommandQueue;
    ComPtr<ID3D12CommandAllocator> m_pCommandAllocator;
    ComPtr<ID3D12GraphicsCommandList> m_pCommandList;
    ComPtr<IDXGISwapChain3> m_pSwapChain;
    ComPtr<ID3D12Fence>  m_pQueueFence;
    ComPtr<IDXGIFactory3> m_pFactory;
    ComPtr<ID3D12DescriptorHeap> m_pDescriptorHeap;
    ComPtr<ID3D12Resource> m_pRenderTarget[2];
    D3D12_CPU_DESCRIPTOR_HANDLE m_rtvHandle[2];
    HANDLE m_queueEvent;
    D3D12_VIEWPORT m_viewPort;
    int m_windowWidth = 0;
    int m_windowHeight = 0;
    float m_backGroundColor[4] = { 0.5f , 0.5f , 0.5f , 1.0f };

    ///<
    /// 
    ///<

    //-----------------------------------------------------------------------------

};
