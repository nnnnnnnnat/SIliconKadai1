//==============================================================================
// Filename: DX12_Graphics.cpp
// Description: DirectX12 �N���X
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

    // �f�o�b�O���C���[��L����
    if (SUCCEEDED(hl)) {
        debug->EnableDebugLayer();
    }
#endif

    // �f�o�C�X�̐���
    auto hr = D3D12CreateDevice(
        nullptr ,
        D3D_FEATURE_LEVEL_11_0 ,
        IID_PPV_ARGS(&m_pDevice));

    if (FAILED(hr)) {
        MessageBoxA(nullptr , "D3D12CreateDevice" , "" , MB_OK);
        return false;
    }

    // �R�}���h�L���[�̐���
    {
        D3D12_COMMAND_QUEUE_DESC desc = {};

        // �R�}���h�L���[�ɓo�^�\�ȃR�}���h���X�g�̃^�C�v�ݒ�
        desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

        // �R�}���h�L���[�̗D��x
        desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;

        // �R�}���h�L���[�𐶐�����ۂ̓����ݒ�t���O
        desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

        // GPU�m�[�h�̎���(GPU����������ꍇ�̂ݐݒ�)
        desc.NodeMask = 0;

        hr = m_pDevice->CreateCommandQueue(&desc , IID_PPV_ARGS(&m_pQueue));

        if (FAILED(hr)) {
            MessageBoxA(nullptr , "CreateCommandQueue" , "" , MB_OK);
            return false;
        }
    }


    // �X���b�v�`�F�C���̐���
    {
        // DXGI�t�@�N�g���[�̐���
        ComPtr<IDXGIFactory4> pFactory = nullptr;
        hr = CreateDXGIFactory1(IID_PPV_ARGS(&pFactory));

        if (FAILED(hr)) {
            MessageBoxA(nullptr , "CreateDXGIFactory1" , "" , MB_OK);
            return false;
        }

        DXGI_SWAP_CHAIN_DESC desc = {};

        // �𑜓x�̉���
        desc.BufferDesc.Width = SCREEN_WIDTH;

        // �𑜓x�̏c��
        desc.BufferDesc.Height = SCREEN_HEIGHT;

        // ���t���b�V�����[�g(Hz�P��)�̕��q���w��
        desc.BufferDesc.RefreshRate.Numerator = 60;

        // ���t���b�V�����[�g(Hz�P��)�̕�����w��
        desc.BufferDesc.RefreshRate.Denominator = 1;

        // �������`�惂�[�h
        desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

        // �g��k���̐ݒ�
        desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

        // �\���`���̃s�N�Z���t�H�[�}�b�g
        desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

        // �s�N�Z���P�ʂ̃}���`�T���v�����O��
        desc.SampleDesc.Count = 1;

        // �摜�̕i�� �i���������قǃp�t�H�[�}���X�͒ቺ
        desc.SampleDesc.Quality = 0;

        // �o�b�N�o�b�t�@�̎g�p���@
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

        // �o�b�N�o�b�t�@�̐�
        desc.BufferCount = FRAME_COUNT;

        // �o�͂���E�B���h�E�̃E�B���h�E�n���h��
        desc.OutputWindow = hWnd;

        // �t���X�N���[���w��
        desc.Windowed = TRUE;

        // �o�b�N�o�b�t�@�̓���ւ�������
        desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

        // �X���b�v�`�F�C���̓���I�v�V����
        desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        // �X���b�v�`�F�C���̐���
        ComPtr<IDXGISwapChain> pSwapChain = nullptr;

        hr = pFactory->CreateSwapChain(m_pQueue.Get() , &desc , &pSwapChain);

        if (FAILED(hr)) {
            MessageBoxA(nullptr , "CreateSwapChain" , "" , MB_OK);
            return false;
        }

        //IDXGISwapChain3���擾
        hr = pSwapChain->QueryInterface(IID_PPV_ARGS(&m_pSwapChain));

        if (FAILED(hr)) {
            MessageBoxA(nullptr , "QueryInterface" , "" , MB_OK);
            return false;
        }

        // �o�b�N�o�b�t�@�ԍ����擾
        m_FrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();
    }


    // �R�}���h�A���P�[�^�̐���
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

    // �R�}���h���X�g�̐���
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

    // �����_�[�^�[�Q�b�g�r���[�̐���
    {
        // �f�B�X�N���v�^�q�[�v�̐ݒ�
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};

        // �q�[�v���̃f�B�X�N���v�^��
        desc.NumDescriptors = FRAME_COUNT;

        // �f�B�X�N���v�^�q�[�v�̃^�C�v
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

        // D3D12_DESCRIPTOR_HEAP_FLAGS�^���r�b�g���Z�ɂ��l�̑g�ݍ��킹�Ŏw��
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        // GPU�m�[�h�̎���(GPU����������ꍇ�̂ݐݒ�)
        desc.NodeMask = 0;

        // �f�B�X�N���v�^�q�[�v�̐���
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

            // ��ʂŌ��鎞�̃s�N�Z���t�H�[�}�b�g�̎w��
            viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

            // �ǂ̂悤�Ƀ����_�[�^�[�Q�b�g�̃��\�[�X�ɃA�N�Z�X���邩�̎������w��
            viewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

            // �g�p����~�b�v�}�b�v���x���̔ԍ�
            viewDesc.Texture2D.MipSlice = 0;

            // �g�p����e�N�X�`���̕��ʃX���C�X�ԍ�
            viewDesc.Texture2D.PlaneSlice = 0;

            // �����_�[�^�[�Q�b�g�r���[�̐���
            m_pDevice->CreateRenderTargetView(m_pColorBuffer[i].Get() , &viewDesc , handle);

            m_HandleRTV[i] = handle;
            handle.ptr += incrementSize;
        }
    }


    // �t�F���X�̐���
    {
        // �t�F���X�J�E���^�[�����Z�b�g
        for (auto i = 0u; i < FRAME_COUNT; i++) {
            m_FenceCounter[i] = 0;
        }

        // �t�F���X�̐���
        hr = m_pDevice->CreateFence(
            m_FenceCounter[m_FrameIndex] ,
            D3D12_FENCE_FLAG_NONE ,
            IID_PPV_ARGS(&m_pFence));

        if (FAILED(hr)) {
            MessageBoxA(nullptr , "CreateFence" , "" , MB_OK);
            return false;
        }

        m_FenceCounter[m_FrameIndex]++;

        // �C�x���g�̐���
        m_FenceEvent = CreateEvent(nullptr , FALSE , FALSE , nullptr);

        if (m_FenceEvent == nullptr) {
            MessageBoxA(nullptr , "m_FenceEvent" , "" , MB_OK);
            return false;
        }
    }

    // �[�x�X�e���V���o�b�t�@�̐���
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

        // �f�B�X�N���v�^�q�[�v�̐ݒ�
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

    // �R�}���h���X�g�����
    m_pCmdList->Close();

    return true;
}

void DX12Graphics::Release() {
    // GPU�����̊�����ҋ@
    SystemWaitGPU();

    // �C�x���g�j��
    if (m_FenceEvent != nullptr) {
        CloseHandle(m_FenceEvent);
        m_FenceEvent = nullptr;
    }

}

void DX12Graphics::BeforeRender() {
    // �R�}���h�̋L�^���J�n
    m_pCmdAllocator[m_FrameIndex]->Reset();
    m_pCmdList->Reset(m_pCmdAllocator[m_FrameIndex].Get() , nullptr);

    // ���\�[�X�o���A�̏������ݐݒ�
    D3D12_RESOURCE_BARRIER barrier = {};

    // ���\�[�X�o���A�̃^�C�v
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

    // �t���O�ݒ�
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

    // �J�ڂɎg�p���郊�\�[�X�̃|�C���^
    barrier.Transition.pResource = m_pColorBuffer[m_FrameIndex].Get();

    // �T�u���\�[�X�̎g�p�O�̏��
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;

    // �T�u���\�[�X�̎g�p��̏��
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

    // �J�ڂׂ̈̃T�u���\�[�X�̔ԍ�
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

    // ���\�[�X�o���A
    m_pCmdList->ResourceBarrier(1 , &barrier);

    // �����_�[�^�[�Q�b�g�̐ݒ�
    m_pCmdList->OMSetRenderTargets(1 , &m_HandleRTV[m_FrameIndex] , FALSE , &m_HandleDSV);

    // �����_�[�^�[�Q�b�g�r���[���N���A
    m_pCmdList->ClearRenderTargetView(m_HandleRTV[m_FrameIndex] , m_backColor , 0 , nullptr);

    // �[�x�X�e���V���r���[���N���A
    m_pCmdList->ClearDepthStencilView(m_HandleDSV , D3D12_CLEAR_FLAG_DEPTH , 1.0f , 0 , 0 , nullptr);
}

void DX12Graphics::AfterRender() {
    // ���\�[�X�o���A�̕\���ݒ�
    D3D12_RESOURCE_BARRIER barrier = {};

    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = m_pColorBuffer[m_FrameIndex].Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

    // ���\�[�X�o���A
    m_pCmdList->ResourceBarrier(1 , &barrier);

    // �R�}���h�̋L�^���I��
    m_pCmdList->Close();

    // �R�}���h�����s
    ID3D12CommandList* ppCmdLists[] = { m_pCmdList.Get() };
    m_pQueue->ExecuteCommandLists(1 , ppCmdLists);

    // ��ʂɕ\��
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

    // �V�O�i������
    m_pQueue->Signal(m_pFence.Get() , m_FenceCounter[m_FrameIndex]);

    // �������ɃC�x���g��ݒ肷��
    m_pFence->SetEventOnCompletion(m_FenceCounter[m_FrameIndex] , m_FenceEvent);

    // �ҋ@����
    WaitForSingleObjectEx(m_FenceEvent , INFINITE , FALSE);

    // �J�E���^�[�𑝂₷
    m_FenceCounter[m_FrameIndex]++;
}

void DX12Graphics::SystemPresent(uint32_t _interval) {
    // ��ʂɕ\��
    m_pSwapChain->Present(_interval , 0);

    // �V�O�i������
    const auto currentValue = m_FenceCounter[m_FrameIndex];
    m_pQueue->Signal(m_pFence.Get() , currentValue);

    // �o�b�N�o�b�t�@�ԍ����X�V
    m_FrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();

    // ���̃t���[���̕`�揀�����܂��ł���Αҋ@����
    if (m_pFence->GetCompletedValue() < m_FenceCounter[m_FrameIndex]) {
        m_pFence->SetEventOnCompletion(m_FenceCounter[m_FrameIndex] , m_FenceEvent);
        WaitForSingleObjectEx(m_FenceEvent , INFINITE , FALSE);
    }

    // ���̃t���[���̃t�F���X�J�E���^�[�𑝂₷
    m_FenceCounter[m_FrameIndex] = currentValue + 1;
}

