#pragma once

// Direct3Dのライブラリを使用できるようにする
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// Direct3Dの型・クラス・関数などを呼べるようにする
#include <d3d11.h>
#include <d3dcompiler.h>

// DirectXMath(数学ライブラリ)を使用できるようにする
#include <DirectXMath.h>

#include <array>

// ComPtrを使用できるようにする
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

struct Vertex {
    DirectX::XMFLOAT3 m_pos;	// 座標
    DirectX::XMFLOAT4 m_color;
    DirectX::XMFLOAT2 m_tex;
};

typedef std::array<Vertex , 4> VertexRectangle;

class Texture {
public:
    bool Init(const float , const float);
    void Draw();
private:
    // 2D描画用のシェーダー
    ComPtr<ID3D11VertexShader>	m_vertexShader = nullptr;	// 頂点シェーダー
    ComPtr<ID3D11PixelShader>	m_pixelShader = nullptr;	// ピクセルシェーダー
    ComPtr<ID3D11InputLayout>	m_inputLayout = nullptr;// 入力レイアウト

    ComPtr<ID3D11Buffer> mp_vertexBuffer;
    ComPtr<ID3D11ShaderResourceView> mp_srv;

    VertexRectangle m_vertex;
};
