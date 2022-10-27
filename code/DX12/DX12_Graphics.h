//==============================================================================
// Filename: DX12_Graohics.h
// Description: DirectX12 クラス
// Copyright (C) Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

#include "../../framework.h"
#include "../Game/Game_Device.h"

#include <cstdint>
#include <d3d12.h>
#include <dxgi1_4.h>

#include <wrl/client.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define FRAME_COUNT 2

class DX12Graphics : public GameDevice {
public:
    //-----------------------------------------------------------------------------
    // public methods

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
    void BeforeRender();

    //-----------------------------------------------------------------------------
    /// 描画後処理
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void AfterRender();

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
    uint32_t GetFrameIndex();

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
    ///<
    //-----------------------------------------------------------------------------
};
