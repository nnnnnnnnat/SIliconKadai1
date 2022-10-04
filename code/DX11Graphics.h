#pragma once

#include <d3d11.h>
#include <wrl/client.h>

#include "Utilities.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

using Microsoft::WRL::ComPtr;

class DX11Graphics : private Util::NonCopyable {
private:
    // DirectX3DDevice11のデバイス
    ComPtr<ID3D11Device> m_device;
    // DirectX3DDevice11のデバイスコンテクスト
    ComPtr<ID3D11DeviceContext> m_deviceContext;
    // スワップチェイン
    ComPtr<IDXGISwapChain> m_swapChain;
    // バックバッファのビュー
    ComPtr<ID3D11RenderTargetView> m_backBufferView;

    // バックバッファXサイズ
    int m_width = 0;
    // バックバッファYサイズ
    int m_height = 0;

private:
    DX11Graphics() {}
public:
    static DX11Graphics& GetInstance() {
        static DX11Graphics instance;
        return instance;
    }

    bool Initialize(HWND , const unsigned int , const unsigned int);
    ID3D11Device* GetDXDevice() const {
        return m_device.Get();
    }
    ID3D11DeviceContext* GetDeviceContext() const {
        return m_deviceContext.Get();
    }
    IDXGISwapChain* GetSwapChain() const {
        return m_swapChain.Get();
    }
    ID3D11RenderTargetView* ID3D11RenderTargetView() const {
        return m_backBufferView.Get();
    }
    int GetViewPortHeight() const {
        return m_height;
    }
    int GetViewPortWidth() const {
        return m_width;
    };
};
