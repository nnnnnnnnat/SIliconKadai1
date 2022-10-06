#pragma once

// Direct3D�̃��C�u�������g�p�ł���悤�ɂ���
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// Direct3D�̌^�E�N���X�E�֐��Ȃǂ��Ăׂ�悤�ɂ���
#include <d3d11.h>
#include <d3dcompiler.h>

// DirectXMath(���w���C�u����)���g�p�ł���悤�ɂ���
#include <DirectXMath.h>

#include <array>

// ComPtr���g�p�ł���悤�ɂ���
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

struct Vertex {
    DirectX::XMFLOAT3 m_pos;	// ���W
    DirectX::XMFLOAT4 m_color;
    DirectX::XMFLOAT2 m_tex;
};

typedef std::array<Vertex , 4> VertexRectangle;

class Texture {
public:
    bool Init(const float , const float);
    void Draw();
private:
    // 2D�`��p�̃V�F�[�_�[
    ComPtr<ID3D11VertexShader>	m_vertexShader = nullptr;	// ���_�V�F�[�_�[
    ComPtr<ID3D11PixelShader>	m_pixelShader = nullptr;	// �s�N�Z���V�F�[�_�[
    ComPtr<ID3D11InputLayout>	m_inputLayout = nullptr;// ���̓��C�A�E�g

    ComPtr<ID3D11Buffer> mp_vertexBuffer;
    ComPtr<ID3D11ShaderResourceView> mp_srv;

    VertexRectangle m_vertex;
};
