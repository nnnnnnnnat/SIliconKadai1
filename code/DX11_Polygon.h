#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <array>
#include <string>

#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

struct VertexPolygon {
    DirectX::XMFLOAT3 m_pos;
    DirectX::XMFLOAT2 m_tex;
    DirectX::XMFLOAT4 m_color;
};

typedef std::array<VertexPolygon , 4> VerteAx;

class DX11Polygon {
public:

    bool Initialize(const float , const float , DirectX::XMFLOAT4);
    void Draw();

private:
    static constexpr LPCWSTR VS_FILENAME = L"shader/SpriteShader.hlsl";
    static constexpr LPCWSTR PS_FILENAME = L"shader/SpriteShader.hlsl";

    // 2D描画用のシェーダー
    ComPtr<ID3D11Buffer> mp_vertexBuffer = nullptr;
    // 頂点シェーダー
    ComPtr<ID3D11VertexShader> mp_vertexShader = nullptr;
    // ピクセルシェーダー
    ComPtr<ID3D11PixelShader> mp_pixelShader = nullptr;
    // 入力レイアウト
    ComPtr<ID3D11InputLayout> mp_inputLayout = nullptr;
    // シェーダーリソースビュー
    ComPtr<ID3D11ShaderResourceView> mp_srv = nullptr;
    // 頂点データ
    VerteAx m_vertex;
};
