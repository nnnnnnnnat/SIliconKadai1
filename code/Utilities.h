//==============================================================================
// Filename: Utilities.h
// Description: 
// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================
#pragma once

#include <Windows.h>
#include <DirectXMath.h>
#include <array>
// 16バイトにアラインメントする。
#define ALIGN16 _declspec(align(16))


class NonCopyable {
protected:
    NonCopyable() {}
    ~NonCopyable() {}
private:
    NonCopyable& operator = (const NonCopyable& src) {};
    NonCopyable(const NonCopyable& src) {};
};

struct Vertex {
    DirectX::XMFLOAT3 m_pos;	// 座標
    DirectX::XMFLOAT4 m_color;
    DirectX::XMFLOAT2 m_tex;
};

struct ConstantBuffer {
    DirectX::XMFLOAT4X4 m_world;
    DirectX::XMFLOAT4X4 m_view;
    DirectX::XMFLOAT4X4 m_projection;
};

typedef std::array<Vertex , 36> VertexCube;
typedef std::array<Vertex , 8> VertexParam;

struct ShaderObject {
    void* binaryPtr;
    int   size;
};

template<typename T> void SafeRelease(T*& _ptr);

