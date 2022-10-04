#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

struct VertexPolygon {
    DirectX::XMFLOAT3 m_pos;
    DirectX::XMFLOAT4 m_color;
};

class DrawPolygon {
    static constexpr const char* VS_FILENAME = "shader/vs.hlsl";
    static constexpr const char* PS_FILENAME = "shader/ps.hlsl";

    // 2D描画用のシェーダー
    // 頂点シェーダー
    ComPtr<ID3D11VertexShader>	m_spriteVS = nullptr;
    // ピクセルシェーダー
    ComPtr<ID3D11PixelShader>	m_spritePS = nullptr;
    // 入力レイアウト
    ComPtr<ID3D11InputLayout>	m_spriteInputLayout = nullptr;

    void Initialize(const float , const float);
};
