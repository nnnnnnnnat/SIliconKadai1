//==============================================================================
// Filename: DX12_Graphics.cpp
// Description: DirectX12 クラス
// Copyright (C) Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#include "DX12_Graphics.h"
#include <Windows.h>
#include <iostream>

using namespace std;

bool DX12Graphics::Init(HWND hWnd) {

#if defined(DEBUG) || defined(_DEBUG)
    ID3D12Debug* debug;

    auto hl = D3D12GetDebugInterface(IID_PPV_ARGS(&debug));

    // デバッグレイヤーを有効化
    if (SUCCEEDED(hl)) {
        debug->EnableDebugLayer();
    }
#endif

    // デバイスの生成
    auto hr = D3D12CreateDevice(
        nullptr ,
        D3D_FEATURE_LEVEL_11_0 ,
        IID_PPV_ARGS(&m_pDevice));

    if (FAILED(hr)) {
        MessageBoxA(nullptr , "D3D12CreateDevice" , "" , MB_OK);
        return false;
    }

    // コマンドキューの生成
    {
        D3D12_COMMAND_QUEUE_DESC desc = {};

        // コマンドキューに登録可能なコマンドリストのタイプ設定
        desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

        // コマンドキューの優先度
        desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;

        // コマンドキューを生成する際の特性設定フラグ
        desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

        // GPUノードの識別(GPUが複数ある場合のみ設定)
        desc.NodeMask = 0;

        hr = m_pDevice->CreateCommandQueue(&desc , IID_PPV_ARGS(&m_pQueue));

        if (FAILED(hr)) {
            MessageBoxA(nullptr , "CreateCommandQueue" , "" , MB_OK);
            return false;
        }
    }


    // スワップチェインの生成
    {
        // DXGIファクトリーの生成
        ComPtr<IDXGIFactory4> pFactory = nullptr;
        hr = CreateDXGIFactory1(IID_PPV_ARGS(&pFactory));

        if (FAILED(hr)) {
            MessageBoxA(nullptr , "CreateDXGIFactory1" , "" , MB_OK);
            return false;
        }

        DXGI_SWAP_CHAIN_DESC desc = {};

        // 解像度の横幅
        desc.BufferDesc.Width = SCREEN_WIDTH;

        // 解像度の縦幅
        desc.BufferDesc.Height = SCREEN_HEIGHT;

        // リフレッシュレート(Hz単位)の分子を指定
        desc.BufferDesc.RefreshRate.Numerator = 60;

        // リフレッシュレート(Hz単位)の分母を指定
        desc.BufferDesc.RefreshRate.Denominator = 1;

        // 走査線描画モード
        desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

        // 拡大縮小の設定
        desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

        // 表示形式のピクセルフォーマット
        desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

        // ピクセル単位のマルチサンプリング数
        desc.SampleDesc.Count = 1;

        // 画像の品質 品質が高いほどパフォーマンスは低下
        desc.SampleDesc.Quality = 0;

        // バックバッファの使用方法
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

        // バックバッファの数
        desc.BufferCount = FRAME_COUNT;

        // 出力するウィンドウのウィンドウハンドル
        desc.OutputWindow = hWnd;

        // フルスクリーン指定
        desc.Windowed = TRUE;

        // バックバッファの入れ替え時効果
        desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

        // スワップチェインの動作オプション
        desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        // スワップチェインの生成
        ComPtr<IDXGISwapChain> pSwapChain = nullptr;

        hr = pFactory->CreateSwapChain(m_pQueue.Get() , &desc , &pSwapChain);

        if (FAILED(hr)) {
            MessageBoxA(nullptr , "CreateSwapChain" , "" , MB_OK);
            return false;
        }

        //IDXGISwapChain3を取得
        hr = pSwapChain->QueryInterface(IID_PPV_ARGS(&m_pSwapChain));

        if (FAILED(hr)) {
            MessageBoxA(nullptr , "QueryInterface" , "" , MB_OK);
            return false;
        }

        // バックバッファ番号を取得
        m_FrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();
    }


    // コマンドアロケータの生成
    {
        for (auto i = 0u; i < FRAME_COUNT; i++) {
            hr = m_pDevice->CreateCommandAllocator(
                D3D12_COMMAND_LIST_TYPE_DIRECT ,
                IID_PPV_ARGS(&m_pCmdAllocator[i]));

            if (FAILED(hr)) {
                MessageBoxA(nullptr , "CreateCommandAllocator" , "" , MB_OK);
                return false;
            }
        }
    }

    // コマンドリストの生成
    {
        hr = m_pDevice->CreateCommandList(
            0 ,
            D3D12_COMMAND_LIST_TYPE_DIRECT ,
            m_pCmdAllocator[m_FrameIndex].Get() ,
            nullptr ,
            IID_PPV_ARGS(&m_pCmdList));

        if (FAILED(hr)) {
            MessageBoxA(nullptr , "CreateCommandList" , "" , MB_OK);
            return false;
        }
    }

    // レンダーターゲットビューの生成
    {
        // ディスクリプタヒープの設定
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};

        // ヒープ内のディスクリプタ数
        desc.NumDescriptors = FRAME_COUNT;

        // ディスクリプタヒープのタイプ
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

        // D3D12_DESCRIPTOR_HEAP_FLAGS型をビット演算による値の組み合わせで指定
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        // GPUノードの識別(GPUが複数ある場合のみ設定)
        desc.NodeMask = 0;

        // ディスクリプタヒープの生成
        hr = m_pDevice->CreateDescriptorHeap(&desc , IID_PPV_ARGS(&m_pHeadRTV));

        if (FAILED(hr)) {
            MessageBoxA(nullptr , "CreateDescriptorHeap" , "" , MB_OK);
            return false;
        }

        auto handle = m_pHeadRTV->GetCPUDescriptorHandleForHeapStart();
        auto incrementSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        for (auto i = 0u; i < FRAME_COUNT; i++) {
            hr = m_pSwapChain->GetBuffer(i , IID_PPV_ARGS(&m_pColorBuffer[i]));

            if (FAILED(hr)) {
                MessageBoxA(nullptr , "GetBuffer" , "" , MB_OK);
                return false;
            }

            D3D12_RENDER_TARGET_VIEW_DESC viewDesc = {};

            // 画面で見る時のピクセルフォーマットの指定
            viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

            // どのようにレンダーターゲットのリソースにアクセスするかの次元を指定
            viewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

            // 使用するミップマップレベルの番号
            viewDesc.Texture2D.MipSlice = 0;

            // 使用するテクスチャの平面スライス番号
            viewDesc.Texture2D.PlaneSlice = 0;

            // レンダーターゲットビューの生成
            m_pDevice->CreateRenderTargetView(m_pColorBuffer[i].Get() , &viewDesc , handle);

            m_HandleRTV[i] = handle;
            handle.ptr += incrementSize;
        }
    }


    // フェンスの生成
    {
        // フェンスカウンターをリセット
        for (auto i = 0u; i < FRAME_COUNT; i++) {
            m_FenceCounter[i] = 0;
        }

        // フェンスの生成
        hr = m_pDevice->CreateFence(
            m_FenceCounter[m_FrameIndex] ,
            D3D12_FENCE_FLAG_NONE ,
            IID_PPV_ARGS(&m_pFence));

        if (FAILED(hr)) {
            MessageBoxA(nullptr , "CreateFence" , "" , MB_OK);
            return false;
        }

        m_FenceCounter[m_FrameIndex]++;

        // イベントの生成
        m_FenceEvent = CreateEvent(nullptr , FALSE , FALSE , nullptr);

        if (m_FenceEvent == nullptr) {
            MessageBoxA(nullptr , "m_FenceEvent" , "" , MB_OK);
            return false;
        }
    }

    // 深度ステンシルバッファの生成
    {
        D3D12_HEAP_PROPERTIES prop = {};

        prop.Type = D3D12_HEAP_TYPE_DEFAULT;
        prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        prop.CreationNodeMask = 1;
        prop.VisibleNodeMask = 1;

        D3D12_RESOURCE_DESC resDesc = {};

        resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        resDesc.Alignment = 0;
        resDesc.Width = SCREEN_WIDTH;
        resDesc.Height = SCREEN_HEIGHT;
        resDesc.DepthOrArraySize = 1;
        resDesc.MipLevels = 1;
        resDesc.Format = DXGI_FORMAT_D32_FLOAT;
        resDesc.SampleDesc.Count = 1;
        resDesc.SampleDesc.Quality = 0;
        resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

        D3D12_CLEAR_VALUE clearValue;

        clearValue.Format = DXGI_FORMAT_D32_FLOAT;
        clearValue.DepthStencil.Depth = 1.0;
        clearValue.DepthStencil.Stencil = 0;

        hr = DX12Graphics::GetDevice()->CreateCommittedResource(
            &prop ,
            D3D12_HEAP_FLAG_NONE ,
            &resDesc ,
            D3D12_RESOURCE_STATE_DEPTH_WRITE ,
            &clearValue ,
            IID_PPV_ARGS(&m_pDepthBuffer));

        if (FAILED(hr)) {
            MessageBoxA(nullptr , "CreateCommittedResource" , "" , MB_OK);
            return false;
        }

        // ディスクリプタヒープの設定
        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};

        heapDesc.NumDescriptors = 1;
        heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        heapDesc.NodeMask = 0;

        hr = m_pDevice->CreateDescriptorHeap(
            &heapDesc ,
            IID_PPV_ARGS(&m_pHeapDSV));

        if (FAILED(hr)) {
            MessageBoxA(nullptr , "CreateDescriptorHeap" , "" , MB_OK);
            return false;
        }

        auto handle = m_pHeapDSV->GetCPUDescriptorHandleForHeapStart();
        auto incrementSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

        D3D12_DEPTH_STENCIL_VIEW_DESC viewDesc = {};

        viewDesc.Format = DXGI_FORMAT_D32_FLOAT;
        viewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
        viewDesc.Texture2D.MipSlice = 0;
        viewDesc.Flags = D3D12_DSV_FLAG_NONE;

        m_pDevice->CreateDepthStencilView(m_pDepthBuffer.Get() , &viewDesc , handle);

        m_HandleDSV = handle;
    }

    // コマンドリストを閉じる
    m_pCmdList->Close();

    return true;
}

void DX12Graphics::Release() {
    // GPU処理の完了を待機
    SystemWaitGPU();

    // イベント破棄
    if (m_FenceEvent != nullptr) {
        CloseHandle(m_FenceEvent);
        m_FenceEvent = nullptr;
    }

}

void DX12Graphics::BeforeRender() {
    // コマンドの記録を開始
    m_pCmdAllocator[m_FrameIndex]->Reset();
    m_pCmdList->Reset(m_pCmdAllocator[m_FrameIndex].Get() , nullptr);

    // リソースバリアの書き込み設定
    D3D12_RESOURCE_BARRIER barrier = {};

    // リソースバリアのタイプ
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

    // フラグ設定
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

    // 遷移に使用するリソースのポインタ
    barrier.Transition.pResource = m_pColorBuffer[m_FrameIndex].Get();

    // サブリソースの使用前の状態
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;

    // サブリソースの使用後の状態
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

    // 遷移の為のサブリソースの番号
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

    // リソースバリア
    m_pCmdList->ResourceBarrier(1 , &barrier);

    // レンダーターゲットの設定
    m_pCmdList->OMSetRenderTargets(1 , &m_HandleRTV[m_FrameIndex] , FALSE , &m_HandleDSV);

    // レンダーターゲットビューをクリア
    m_pCmdList->ClearRenderTargetView(m_HandleRTV[m_FrameIndex] , m_backColor , 0 , nullptr);

    // 深度ステンシルビューをクリア
    m_pCmdList->ClearDepthStencilView(m_HandleDSV , D3D12_CLEAR_FLAG_DEPTH , 1.0f , 0 , 0 , nullptr);
}

void DX12Graphics::AfterRender() {
    // リソースバリアの表示設定
    D3D12_RESOURCE_BARRIER barrier = {};

    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = m_pColorBuffer[m_FrameIndex].Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

    // リソースバリア
    m_pCmdList->ResourceBarrier(1 , &barrier);

    // コマンドの記録を終了
    m_pCmdList->Close();

    // コマンドを実行
    ID3D12CommandList* ppCmdLists[] = { m_pCmdList.Get() };
    m_pQueue->ExecuteCommandLists(1 , ppCmdLists);

    // 画面に表示
    SystemPresent(1);
}

ID3D12Device* DX12Graphics::GetDevice() {
    return m_pDevice.Get();
}

ID3D12CommandQueue* DX12Graphics::GetCommandQueue() {
    return m_pQueue.Get();
}

ID3D12GraphicsCommandList* DX12Graphics::GetCommandList() {
    return m_pCmdList.Get();
}

uint32_t DX12Graphics::GetFrameIndex() {
    return m_FrameIndex;
}

void DX12Graphics::SystemWaitGPU() {

    // シグナル処理
    m_pQueue->Signal(m_pFence.Get() , m_FenceCounter[m_FrameIndex]);

    // 完了時にイベントを設定する
    m_pFence->SetEventOnCompletion(m_FenceCounter[m_FrameIndex] , m_FenceEvent);

    // 待機処理
    WaitForSingleObjectEx(m_FenceEvent , INFINITE , FALSE);

    // カウンターを増やす
    m_FenceCounter[m_FrameIndex]++;
}

void DX12Graphics::SystemPresent(uint32_t _interval) {
    // 画面に表示
    m_pSwapChain->Present(_interval , 0);

    // シグナル処理
    const auto currentValue = m_FenceCounter[m_FrameIndex];
    m_pQueue->Signal(m_pFence.Get() , currentValue);

    // バックバッファ番号を更新
    m_FrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();

    // 次のフレームの描画準備がまだであれば待機する
    if (m_pFence->GetCompletedValue() < m_FenceCounter[m_FrameIndex]) {
        m_pFence->SetEventOnCompletion(m_FenceCounter[m_FrameIndex] , m_FenceEvent);
        WaitForSingleObjectEx(m_FenceEvent , INFINITE , FALSE);
    }

    // 次のフレームのフェンスカウンターを増やす
    m_FenceCounter[m_FrameIndex] = currentValue + 1;
}

