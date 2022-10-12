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
    /// \return ID3D12Device
    //-----------------------------------------------------------------------------
    ID3D12Device* GetDevice();

    //-----------------------------------------------------------------------------

private:
    //-----------------------------------------------------------------------------
    // private method

    //-----------------------------------------------------------------------------
    /// ���\�[�X�o���A�ݒ�
    /// 
    /// \param [out] _pRommandList �R�}���h���X�g
    /// \param [in] _pResource �����_�[�^�[�Q�b�g
    /// \param [in] _before ���\�[�X�X�e�[�g�̓����̂��߂̈���
    /// \param [in] _after ���\�[�X�X�e�[�g�̓����̂��߂̈���
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
