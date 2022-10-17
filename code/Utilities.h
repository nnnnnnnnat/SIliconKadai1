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

// コピーコンストラクタを禁止にするためのクラス
class NonCopyable {
protected:
    //-----------------------------------------------------------------------------
    // protected method

    //-----------------------------------------------------------------------------
    /// コンストラクタ
    //-----------------------------------------------------------------------------
    NonCopyable() {}

    //-----------------------------------------------------------------------------
    /// デストラクタ
    //-----------------------------------------------------------------------------
    ~NonCopyable() {}
private:
    //-----------------------------------------------------------------------------
    // private method
    NonCopyable& operator = (const NonCopyable& src) {};
    NonCopyable(const NonCopyable& src) {};
    //-----------------------------------------------------------------------------

};

struct Vertex {
    DirectX::XMFLOAT3 m_pos;
    DirectX::XMFLOAT4 m_color;
    DirectX::XMFLOAT2 m_tex;
};

struct ConstantBuffer {
    DirectX::XMFLOAT4X4 m_world;
    DirectX::XMFLOAT4X4 m_view;
    DirectX::XMFLOAT4X4 m_projection;
};

typedef std::array<Vertex , 36> VertexCube;

struct ShaderObject {
    void* binaryPtr;
    int   size;
};

template<typename T> void SafeRelease(T*& _ptr);
VertexCube CubeIdentify();
VertexCube CubeIdentify(const float ratio);
