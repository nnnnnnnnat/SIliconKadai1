//==============================================================================
// Filename: DX12_Graohics.h
// Description: DirectX12 クラス
// Copyright (C) Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

#include "../../framework.h"

#include <cstdint>
#include <d3d12.h>
#include <dxgi1_4.h>

#include <wrl/client.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define FRAME_COUNT 2

class DX12Graphics : private NonCopyable {
public:
    //-----------------------------------------------------------------------------
    // public methods

    //-----------------------------------------------------------------------------
    /// インスタンス取得
    /// 
    /// \return SystemDirectX12&
    //-----------------------------------------------------------------------------
    static DX12Graphics& GetInstance();

    //-----------------------------------------------------------------------------
    /// 初期化
    /// 
    /// \param [in] hWnd ウィンドウハンドル
    /// 
    /// \return bool
    //-----------------------------------------------------------------------------
    bool Init(HWND hWnd);		/// 初期化処理

    //-----------------------------------------------------------------------------
    /// 解放処理
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Release();

    //-----------------------------------------------------------------------------
    /// 描画前処理
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void SystemBeforeRender();

    //-----------------------------------------------------------------------------
    /// 描画後処理
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void SystemAfterRender();

    //-----------------------------------------------------------------------------
    /// デバイス取得
    /// 
    /// \return ID3D12Device*
    //-----------------------------------------------------------------------------
    ID3D12Device* GetDevice();

    //-----------------------------------------------------------------------------
    /// コマンドキュー取得
    /// 
    /// \return ID3D12CommandQueue*
    //-----------------------------------------------------------------------------
    ID3D12CommandQueue* GetCommandQueue();

    //-----------------------------------------------------------------------------
    /// コマンドリスト取得
    /// 
    /// \return ID3D12GraphicsCommandList*
    //-----------------------------------------------------------------------------
    ID3D12GraphicsCommandList* GetCommandList();

    //-----------------------------------------------------------------------------
    /// フレーム番号取得
    /// 
    /// \return uint32_t
    //-----------------------------------------------------------------------------
    uint32_t SystemGetFrameIndex();

    //-----------------------------------------------------------------------------
    /// 背景色設定
    /// 
    /// \param [in] _color 背景色
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void SetBackGroundColor(float _color[4]);

    //-----------------------------------------------------------------------------
private:
    //-----------------------------------------------------------------------------
    // private method

    //-----------------------------------------------------------------------------
    /// GPU待機処理
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void SystemWaitGPU();

    //-----------------------------------------------------------------------------
    /// メイン描画処理
    /// 
    /// \param _interval 1を入れる
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void SystemPresent(uint32_t _interval);

    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // private variable

    ComPtr<ID3D12Device> m_pDevice;
    ComPtr<ID3D12CommandQueue> m_pQueue;
    ComPtr<IDXGISwapChain3> m_pSwapChain;
    ComPtr<ID3D12Resource> m_pColorBuffer[FRAME_COUNT];
    ComPtr<ID3D12CommandAllocator> m_pCmdAllocator[FRAME_COUNT];
    ComPtr<ID3D12GraphicsCommandList> m_pCmdList;
    ComPtr<ID3D12DescriptorHeap> m_pHeadRTV;
    ComPtr<ID3D12Fence> m_pFence;
    ComPtr<ID3D12Resource> m_pDepthBuffer;
    ComPtr<ID3D12DescriptorHeap> m_pHeapDSV;

    HANDLE m_FenceEvent;
    uint64_t m_FenceCounter[FRAME_COUNT];
    uint32_t m_FrameIndex;

    D3D12_CPU_DESCRIPTOR_HANDLE	m_HandleRTV[FRAME_COUNT];
    D3D12_CPU_DESCRIPTOR_HANDLE	m_HandleDSV;

    float m_backGroundColor[4] = { 0.2f , 0.2f , 0.2f , 1.0f };

    ///<
    /// m_pDevice デバイス
    /// m_pQueue コマンドキュー
    /// m_pSwapChain スワップチェイン
    /// m_pColorBuffer カラーバッファ
    /// m_pCmdAllocator コマンドアロケータ
    /// m_pCmdList コマンドリスト
    /// m_pHeadRTV レンダーターゲットビュー用ヒープ
    /// m_pFence フェンス
    /// m_pDepthBuffer ステンシルバッファ
    /// m_pHeapDSV デプスステンシルビュー用ヒープ
    /// 
    /// m_FenceEvent フェンスイベント
    /// m_FenceCounter フェンスカウンター
    /// m_FrameIndex フレーム番号
    /// 
    /// m_HandleRTV レンダーターゲットビュー用ハンドル
    /// m_HandleDSV デプスステンシルビュー用ハンドル
    /// 
    /// m_backGroundColor 背景色
    ///<
    //-----------------------------------------------------------------------------
};
