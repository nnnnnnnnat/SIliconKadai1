//==============================================================================
// Filename: DX12_Graohics.h
// Description: DirectX12 �N���X
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
    /// ������
    /// 
    /// \param [in] hWnd �E�B���h�E�n���h��
    /// 
    /// \return bool
    //-----------------------------------------------------------------------------
    bool Init(HWND hWnd);		/// ����������

    //-----------------------------------------------------------------------------
    /// �������
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Release();

    //-----------------------------------------------------------------------------
    /// �`��O����
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void BeforeRender();

    //-----------------------------------------------------------------------------
    /// �`��㏈��
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void AfterRender();

    //-----------------------------------------------------------------------------
    /// �f�o�C�X�擾
    /// 
    /// \return ID3D12Device*
    //-----------------------------------------------------------------------------
    ID3D12Device* GetDevice();

    //-----------------------------------------------------------------------------
    /// �R�}���h�L���[�擾
    /// 
    /// \return ID3D12CommandQueue*
    //-----------------------------------------------------------------------------
    ID3D12CommandQueue* GetCommandQueue();

    //-----------------------------------------------------------------------------
    /// �R�}���h���X�g�擾
    /// 
    /// \return ID3D12GraphicsCommandList*
    //-----------------------------------------------------------------------------
    ID3D12GraphicsCommandList* GetCommandList();

    //-----------------------------------------------------------------------------
    /// �t���[���ԍ��擾
    /// 
    /// \return uint32_t
    //-----------------------------------------------------------------------------
    uint32_t GetFrameIndex();

    //-----------------------------------------------------------------------------
private:
    //-----------------------------------------------------------------------------
    // private method

    //-----------------------------------------------------------------------------
    /// GPU�ҋ@����
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void SystemWaitGPU();

    //-----------------------------------------------------------------------------
    /// ���C���`�揈��
    /// 
    /// \param _interval 1������
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
    /// m_pDevice �f�o�C�X
    /// m_pQueue �R�}���h�L���[
    /// m_pSwapChain �X���b�v�`�F�C��
    /// m_pColorBuffer �J���[�o�b�t�@
    /// m_pCmdAllocator �R�}���h�A���P�[�^
    /// m_pCmdList �R�}���h���X�g
    /// m_pHeadRTV �����_�[�^�[�Q�b�g�r���[�p�q�[�v
    /// m_pFence �t�F���X
    /// m_pDepthBuffer �X�e���V���o�b�t�@
    /// m_pHeapDSV �f�v�X�X�e���V���r���[�p�q�[�v
    /// 
    /// m_FenceEvent �t�F���X�C�x���g
    /// m_FenceCounter �t�F���X�J�E���^�[
    /// m_FrameIndex �t���[���ԍ�
    /// 
    /// m_HandleRTV �����_�[�^�[�Q�b�g�r���[�p�n���h��
    /// m_HandleDSV �f�v�X�X�e���V���r���[�p�n���h��
    ///<
    //-----------------------------------------------------------------------------
};
