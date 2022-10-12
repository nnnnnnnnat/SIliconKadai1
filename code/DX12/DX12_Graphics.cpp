#include "DX12_Graphics.h"

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

    hr = CreateDXGIFactory2(dxgiFlags ,
        IID_PPV_ARGS(m_pFactory.ReleaseAndGetAddressOf()));
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "CreateDXGIFactory2" , "" , MB_OK);
        return false;
    }

    ComPtr<IDXGIAdapter> pAdapter;
    hr = m_pFactory->EnumAdapters(0 , pAdapter.GetAddressOf());
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "EnumAdapters" , "" , MB_OK);
        return false;
    }

    // デバイス生成
    hr = D3D12CreateDevice(pAdapter.Get() ,
        D3D_FEATURE_LEVEL_11_0 ,
        IID_PPV_ARGS(&m_pDevice));
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "D3D12CreateDevice" , "" , MB_OK);
        return false;
    }

    // コマンドアロケータ生成
    hr = m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT ,
        IID_PPV_ARGS(&m_pCommandAllocator));
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "CreateCommandAllocator" , "" , MB_OK);
        return false;
    }

    // コマンドキュー生成
    D3D12_COMMAND_QUEUE_DESC commandQueueDesc;
    ZeroMemory(&commandQueueDesc , sizeof(commandQueueDesc));
    commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    commandQueueDesc.Priority = 0;
    commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    hr = m_pDevice->CreateCommandQueue(&commandQueueDesc ,
        IID_PPV_ARGS(m_pCommandQueue.GetAddressOf()));
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "CreateCommandQueue" , "" , MB_OK);
        return false;
    }

    // コマンドキュー用のフェンスを準備
    m_queueEvent = CreateEvent(NULL , FALSE , FALSE , NULL);
    hr = m_pDevice->CreateFence(0 , D3D12_FENCE_FLAG_NONE ,
        IID_PPV_ARGS(m_pQueueFence.GetAddressOf()));
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "CreateFence" , "" , MB_OK);
        return false;
    }

    // スワップチェイン生成
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc , sizeof(swapChainDesc));
    swapChainDesc.BufferCount = 2;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = _hWnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    hr = m_pFactory->CreateSwapChain(m_pCommandQueue.Get() ,
        &swapChainDesc ,
        (IDXGISwapChain**)m_pSwapChain.GetAddressOf());
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "CreateSwapChain" , "" , MB_OK);
        return false;
    }

    // コマンドリスト生成
    hr = m_pDevice->CreateCommandList(0 , D3D12_COMMAND_LIST_TYPE_DIRECT ,
        m_pCommandAllocator.Get() ,
        nullptr ,
        IID_PPV_ARGS(m_pCommandList.GetAddressOf()));
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "CreateCommandList" , "" , MB_OK);
        return false;
    }

    // ディスクリプタヒープ生成
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
    ZeroMemory(&heapDesc , sizeof(heapDesc));
    heapDesc.NumDescriptors = 2;
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    hr = m_pDevice->CreateDescriptorHeap(&heapDesc ,
        IID_PPV_ARGS(m_pDescriptorHeap.GetAddressOf()));
    if (FAILED(hr)) { // if
        MessageBox(nullptr , "CreateDescriptorHeap" , "" , MB_OK);
        return false;
    }

    // レンダーターゲット生成
    UINT strideHandleByte =
        m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    for (int i = 0; i < swapChainDesc.BufferCount; i++) {
        hr = m_pSwapChain->GetBuffer(i ,
            IID_PPV_ARGS(m_pRenderTarget[i].GetAddressOf()));
        if (FAILED(hr)) { // if
            MessageBox(nullptr , "GetBuffer" , "" , MB_OK);
            return false;
        }
        m_rtvHandle[i] = m_pDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
        m_rtvHandle[i].ptr += i * strideHandleByte;
        m_pDevice->CreateRenderTargetView(m_pRenderTarget[i].Get() ,
            nullptr ,
            m_rtvHandle[i]);
    }

    // ビューポート設定
    m_viewPort.TopLeftX = 0;
    m_viewPort.TopLeftY = 0;
    m_viewPort.Width = (FLOAT)m_windowWidth;
    m_viewPort.Height = (FLOAT)m_windowHeight;
    m_viewPort.MinDepth = 0;
    m_viewPort.MaxDepth = 1;

    return true;
}

void DX12Graphics::Update() {

}

void DX12Graphics::BeforeRender() {
    int targetIdx = m_pSwapChain->GetCurrentBackBufferIndex();

    SetResourceBarrier(
        m_pCommandList.Get() ,
        m_pRenderTarget[targetIdx].Get() ,
        D3D12_RESOURCE_STATE_PRESENT ,
        D3D12_RESOURCE_STATE_RENDER_TARGET);

    // レンダーターゲットのクリア処理.
    m_pCommandList->RSSetViewports(1 , &m_viewPort);
    m_pCommandList->ClearRenderTargetView(m_rtvHandle[targetIdx] ,
        m_backGroundColor ,
        0 ,
        nullptr);

    // Presentする前の準備.
    SetResourceBarrier(
        m_pCommandList.Get() ,
        m_pRenderTarget[targetIdx].Get() ,
        D3D12_RESOURCE_STATE_RENDER_TARGET ,
        D3D12_RESOURCE_STATE_PRESENT);

    m_pCommandList->Close();
}

void DX12Graphics::AfterRender() {
    // 積んだコマンドの実行.
    ID3D12CommandList* pCommandList = m_pCommandList.Get();
    m_pCommandQueue->ExecuteCommandLists(1 , &pCommandList);
    m_pSwapChain->Present(1 , 0);

    WaitForCommandQueue(m_pCommandQueue.Get());
    m_pCommandAllocator->Reset();
    m_pCommandList->Reset(m_pCommandAllocator.Get() , nullptr);

}

void DX12Graphics::Release() {

}

ID3D12Device* DX12Graphics::GetDevice() {
    return m_pDevice.Get();
}

void DX12Graphics::SetResourceBarrier(
    ID3D12GraphicsCommandList* _pRommandList ,
    ID3D12Resource* _pResource ,
    D3D12_RESOURCE_STATES _before ,
    D3D12_RESOURCE_STATES _after) {

    D3D12_RESOURCE_BARRIER descBarrier;
    ZeroMemory(&descBarrier , sizeof(descBarrier));
    descBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    descBarrier.Transition.pResource = _pResource;
    descBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    descBarrier.Transition.StateBefore = _before;
    descBarrier.Transition.StateAfter = _after;
    _pRommandList->ResourceBarrier(1 , &descBarrier);

}

void DX12Graphics::WaitForCommandQueue(
    ID3D12CommandQueue* _pCommandQueue) {
    static UINT64 s_frames = 0;
    m_pQueueFence->SetEventOnCompletion(s_frames , m_queueEvent);
    _pCommandQueue->Signal(m_pQueueFence.Get() , s_frames);
    WaitForSingleObject(m_queueEvent , INFINITE);
    s_frames++;
}

void DX12Graphics::SetBackGroundColor(float _color[4]) {
    for (int i = 0; i < 4; i++) {
        m_backGroundColor[i] = _color[i];
    }
}
