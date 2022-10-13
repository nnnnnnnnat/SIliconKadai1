#include "DX12_Graphics.h"

/*
DirectX12 の描画について
1. CommandList にコマンドを積む
2. CommandList を Close
3. CommandQueue に CommandList を渡して実行
4. CommandAllocator を Reset
5. CommandList を Reset
という順序で行われる
*/

DX12Graphics& DX12Graphics::GetInstance() {
    static DX12Graphics instance;
    return instance;
}

bool DX12Graphics::Init(HWND _hWnd , const UINT _width , const UINT _height) {

    m_windowWidth = _width;
    m_windowHeight = _height;

    HRESULT hr;
    UINT dxgiFlags = 0;
    ID3D12Debug* debug = nullptr;

#if _DEBUG

    D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
    if (debug) { // if
        debug->EnableDebugLayer();
        debug->Release();
    }
    dxgiFlags |= DXGI_CREATE_FACTORY_DEBUG;

#endif

    // ファクトリー作成
    hr = CreateDXGIFactory2(dxgiFlags , IID_PPV_ARGS(m_pFactory.ReleaseAndGetAddressOf()));
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "CreateDXGIFactory2" , "" , MB_OK);
    }

    // アダプター作成
    ComPtr<IDXGIAdapter> pAdapter;
    hr = m_pFactory->EnumAdapters(0 , pAdapter.GetAddressOf());
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "EnumAdapters" , "" , MB_OK);
    }

    // デバイス作成
    hr = D3D12CreateDevice(pAdapter.Get() , D3D_FEATURE_LEVEL_11_0 , IID_PPV_ARGS(m_pDevice.GetAddressOf()));
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "D3D12CreateDevice" , "" , MB_OK);
    }

    D3D12_FEATURE_DATA_D3D12_OPTIONS options;
    hr = m_pDevice->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS , (void*)&options , sizeof(options));
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "CheckFeatureSupport" , "" , MB_OK);
    }

    // コマンドアロケータ作成
    hr = m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT , IID_PPV_ARGS(m_pCommandAllocator.GetAddressOf()));
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "CreateCommandAllocator" , "" , MB_OK);
    }

    // コマンドキュー作成
    D3D12_COMMAND_QUEUE_DESC descCommandQueue;
    ZeroMemory(&descCommandQueue , sizeof(descCommandQueue));
    descCommandQueue.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    descCommandQueue.Priority = 0;
    descCommandQueue.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    hr = m_pDevice->CreateCommandQueue(&descCommandQueue , IID_PPV_ARGS(m_pCommandQueue.GetAddressOf()));
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "CreateCommandQueue" , "" , MB_OK);
    }

    // コマンドキュー用のフェンス作成
    m_queueEvent = CreateEvent(NULL , FALSE , FALSE , NULL);
    hr = m_pDevice->CreateFence(0 , D3D12_FENCE_FLAG_NONE , IID_PPV_ARGS(m_pQueueFence.GetAddressOf()));
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "CreateFence" , "" , MB_OK);
    }

    // スワップチェインを生成.
    DXGI_SWAP_CHAIN_DESC descSwapChain;
    ZeroMemory(&descSwapChain , sizeof(descSwapChain));
    descSwapChain.BufferCount = 2;
    descSwapChain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    descSwapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    descSwapChain.OutputWindow = _hWnd;
    descSwapChain.SampleDesc.Count = 1;
    descSwapChain.Windowed = TRUE;
    descSwapChain.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    descSwapChain.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    hr = m_pFactory->CreateSwapChain(m_pCommandQueue.Get() , &descSwapChain , (IDXGISwapChain**)m_pSwapChain.GetAddressOf());
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "CreateSwapChain" , "" , MB_OK);
    }

    // コマンドリスト作成
    hr = m_pDevice->CreateCommandList(0 , D3D12_COMMAND_LIST_TYPE_DIRECT , m_pCommandAllocator.Get() , nullptr , IID_PPV_ARGS(m_pCommandList.GetAddressOf()));
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "CreateCommandList" , "" , MB_OK);
    }

    // ディスクリプタヒープ(RenderTarget用)の作成
    D3D12_DESCRIPTOR_HEAP_DESC descHeap;
    ZeroMemory(&descHeap , sizeof(descHeap));
    descHeap.NumDescriptors = 2;
    descHeap.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    descHeap.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    hr = m_pDevice->CreateDescriptorHeap(&descHeap , IID_PPV_ARGS(m_pDescriptorHeapRTV.GetAddressOf()));
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "CreateDescriptorHeap" , "" , MB_OK);
    }

    // レンダーターゲット(プライマリ用)の作成.
    UINT strideHandleBytes = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    for (UINT i = 0; i < descSwapChain.BufferCount; ++i) {
        hr = m_pSwapChain->GetBuffer(i , IID_PPV_ARGS(m_pRenderTarget[i].GetAddressOf()));
        if (FAILED(hr)) { // if
            MessageBox(nullptr , "GetBuffer" , "" , MB_OK);
        }
        m_rtvHandle[i] = m_pDescriptorHeapRTV->GetCPUDescriptorHandleForHeapStart();
        m_rtvHandle[i].ptr += i * strideHandleBytes;
        m_pDevice->CreateRenderTargetView(m_pRenderTarget[i].Get() , nullptr , m_rtvHandle[i]);
    }

    // デプスバッファの準備.
    D3D12_DESCRIPTOR_HEAP_DESC descDescriptorHeapDSB;
    ZeroMemory(&descDescriptorHeapDSB , sizeof(descDescriptorHeapDSB));
    descDescriptorHeapDSB.NumDescriptors = 1;
    descDescriptorHeapDSB.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    descDescriptorHeapDSB.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    hr = m_pDevice->CreateDescriptorHeap(
        &descDescriptorHeapDSB ,
        IID_PPV_ARGS(m_pDescriptorHeapDSB.GetAddressOf()));
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "CreateDescriptorHeap" , "" , MB_OK);
    }

    m_pRenderTargetDepth = DX12Util::CreateDepthBuffer(m_pDevice.Get() , m_windowWidth , m_windowHeight);

    if (FAILED(hr)) { // if
        MessageBox(nullptr , "CreateCommittedResource" , "" , MB_OK);
    }


    D3D12_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory(&descDSV , sizeof(descDSV));
    descDSV.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    descDSV.Format = DXGI_FORMAT_D32_FLOAT;
    descDSV.Texture2D.MipSlice = 0;
    m_pDevice->CreateDepthStencilView(
        m_pRenderTargetDepth.Get() ,
        &descDSV ,
        m_pDescriptorHeapDSB->GetCPUDescriptorHandleForHeapStart()
    );

    m_handleDSV = m_pDescriptorHeapDSB->GetCPUDescriptorHandleForHeapStart();

    m_viewPort = { 0.0f ,
        0.0f ,
        (float)m_windowWidth ,
        (float)m_windowHeight ,
        0.0f ,
        1.0f };

    m_rect = {
        0 ,
        0 ,
        m_windowWidth ,
        m_windowHeight ,
    };

    return true;
}

void DX12Graphics::Update() {

}

void DX12Graphics::BeforeRender() {
    int targetIdx = m_pSwapChain->GetCurrentBackBufferIndex();

    DX12Util::SetResourceBarrier(
        m_pCommandList.Get() ,
        m_pRenderTarget[targetIdx].Get() ,
        D3D12_RESOURCE_STATE_PRESENT ,
        D3D12_RESOURCE_STATE_RENDER_TARGET);

    // レンダーターゲットのクリア処理.
    m_pCommandList->ClearDepthStencilView(m_handleDSV , D3D12_CLEAR_FLAG_DEPTH , 1.0f , 0 , 0 , nullptr);
    m_pCommandList->ClearRenderTargetView(m_rtvHandle[targetIdx] , m_backGroundColor , 0 , nullptr);
    m_pCommandList->OMSetRenderTargets(1 , &m_rtvHandle[targetIdx] , TRUE , &m_handleDSV);

}

void DX12Graphics::AfterRender() {
    int targetIdx = m_pSwapChain->GetCurrentBackBufferIndex();

    // Presentする前の準備.
    DX12Util::SetResourceBarrier(
        m_pCommandList.Get() ,
        m_pRenderTarget[targetIdx].Get() ,
        D3D12_RESOURCE_STATE_RENDER_TARGET ,
        D3D12_RESOURCE_STATE_PRESENT);

    // 2. CommandList を Close
    m_pCommandList->Close();

    // 3. CommandQueue に CommandList を渡して実行
    ID3D12CommandList* pCommandList = m_pCommandList.Get();
    m_pCommandQueue->ExecuteCommandLists(1 , &pCommandList);
    m_pSwapChain->Present(1 , 0);
    WaitForCommandQueue(m_pCommandQueue.Get());

    // 4. CommandAllocator を Reset
    m_pCommandAllocator->Reset();
    // 5. CommandList を Reset
    m_pCommandList->Reset(m_pCommandAllocator.Get() , nullptr);

}

void DX12Graphics::Release() {

}

ID3D12Device* DX12Graphics::GetDevice() {
    return m_pDevice.Get();
}

ID3D12GraphicsCommandList* DX12Graphics::GetCommandList() {
    return m_pCommandList.Get();
}

int DX12Graphics::GetSwapChainCurrentBackBufferindex() {
    return m_pSwapChain->GetCurrentBackBufferIndex();
}

D3D12_VIEWPORT DX12Graphics::GetViewPort() {
    return m_viewPort;
}

D3D12_RECT DX12Graphics::GetRect() {
    return m_rect;
}

int DX12Graphics::GetWindowWidth() {
    return m_windowWidth;
}

int DX12Graphics::GetWindowHeight() {
    return m_windowHeight;
}

void DX12Graphics::WaitForCommandQueue(
    ID3D12CommandQueue* _pCommandQueue) {
    static UINT64 s_frames = 0;
    m_pQueueFence->SetEventOnCompletion(s_frames , m_queueEvent);
    _pCommandQueue->Signal(m_pQueueFence.Get() , s_frames);
    WaitForSingleObject(m_queueEvent , INFINITE);
    s_frames++;
}

void DX12Graphics::SetBackGroundColor(const float _color[4]) {
    for (int i = 0; i < 4; i++) {
        m_backGroundColor[i] = _color[i];
    }
}
