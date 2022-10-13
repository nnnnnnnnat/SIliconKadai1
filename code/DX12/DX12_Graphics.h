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
    /// �C���X�^���X�󂯎��
    /// 
    /// \return DX12Graphics&
    //-----------------------------------------------------------------------------
    static DX12Graphics& GetInstance();

    //-----------------------------------------------------------------------------
    /// DX12������
    /// 
    /// \param [in] _hWnd �E�B���h�E�n���h��
    /// \param [in] _width �E�B���h�E�̉��̑傫��
    /// \param [in] _height �E�B���h�E�̏c�̑傫��
    /// 
    /// \return bool
    //-----------------------------------------------------------------------------
    bool Init(HWND _hWnd , const UINT _width , const UINT _height);

    //-----------------------------------------------------------------------------
    /// �X�V
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Update();

    //-----------------------------------------------------------------------------
    /// �`��O����
    /// 
    /// \return void
    //----------------------------------------------------------------------
    void BeforeRender();

    //-----------------------------------------------------------------------------
    /// �`��㏈��
    /// 
    /// \return void
    //----------------------------------------------------------------------
    void AfterRender();

    //-----------------------------------------------------------------------------
    /// �������
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void Release();

    //-----------------------------------------------------------------------------
    /// �f�o�C�X�擾
    /// 
    /// \return ID3D12Device*
    //-----------------------------------------------------------------------------
    ID3D12Device* GetDevice();

    //-----------------------------------------------------------------------------
    /// �R�}���h���X�g�擾
    /// 
    /// \return ID3D12CommandList*
    //-----------------------------------------------------------------------------
    ID3D12GraphicsCommandList* GetCommandList();

    //-----------------------------------------------------------------------------
    /// �o�b�N�o�b�t�@�̔ԍ����擾
    /// 
    /// \return int
    //-----------------------------------------------------------------------------
    int GetSwapChainCurrentBackBufferindex();

    //-----------------------------------------------------------------------------
    /// �r���[�|�[�g�擾
    /// 
    /// \return D3D12_VIEWPORT
    //-----------------------------------------------------------------------------
    D3D12_VIEWPORT GetViewPort();

    //-----------------------------------------------------------------------------
    /// �V�U�[�Z�`�擾
    /// 
    /// \return D3D12_RECT
    //-----------------------------------------------------------------------------
    D3D12_RECT GetRect();

    //-----------------------------------------------------------------------------
    /// �E�B���h�E�̉��̒����擾
    /// 
    /// \return D3D12_RECT
    //-----------------------------------------------------------------------------
    int GetWindowWidth();

    //-----------------------------------------------------------------------------
    /// �E�B���h�E�̏c�̒����擾
    /// 
    /// \return D3D12_RECT
    //-----------------------------------------------------------------------------
    int GetWindowHeight();

    //-----------------------------------------------------------------------------

private:
    //-----------------------------------------------------------------------------
    // private method

    //-----------------------------------------------------------------------------
    /// �`��I���҂��֐�
    /// 
    /// \param [out] _pCommandQueue �R�}���h�L���[
    /// 
    /// \return void
    //-----------------------------------------------------------------------------
    void WaitForCommandQueue(
        ID3D12CommandQueue* _pCommandQueue);

    //-----------------------------------------------------------------------------
    /// �w�i�F�ݒ�
    /// 
    /// \param [in] _color[4] �w�i�F
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
    /// m_pDevice DX12�f�o�C�X
    /// m_pSwapChain �X���b�v�`�F�C��
    /// m_pFactory �t�@�N�g���[
    /// m_pCommandAllocator �R�}���h�A���P�[�^�\
    /// m_pCommandQueue �R�}���h�L���[
    /// m_pQueueFence �t�F���X
    /// m_pDescriptorHeap �f�B�X�N���v�^�q�[�v
    /// m_pCommandList �O���t�B�b�N�R�}���h���X�g
    ///<

    //-----------------------------------------------------------------------------

};
