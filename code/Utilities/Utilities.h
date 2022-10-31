//==============================================================================
/// Filename: Utilities.h
/// Description: ���ʂ̃��\�b�h
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================
#pragma once

#include <Windows.h>
#include <DirectXMath.h>
#include <array>

// 16�o�C�g�ɃA���C�������g����B
#define ALIGN16 _declspec(align(16))

// �R�s�[�R���X�g���N�^���֎~�ɂ��邽�߂̃N���X
class NonCopyable {
protected:
    //-----------------------------------------------------------------------------
    // protected method

    //-----------------------------------------------------------------------------
    /// �R���X�g���N�^
    //-----------------------------------------------------------------------------
    NonCopyable() {}

    //-----------------------------------------------------------------------------
    /// �f�X�g���N�^
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

//-----------------------------------------------------------------------------
/// �|�C���^�̊J�[����
/// 
/// \param  [out]   _ptr    ����������|�C���^
/// 
/// \return void
//-----------------------------------------------------------------------------
template<typename T> void SafeRelease(
    /*[out]*/ T*& _ptr);

//-----------------------------------------------------------------------------
/// �L���[�u���̏�����
/// 
/// \return VertexCube
//-----------------------------------------------------------------------------
VertexCube CubeIdentify();

//-----------------------------------------------------------------------------
/// �L���[�u���̏�����
/// 
/// \param  [in]    ratio   �傫���̔{��
/// 
/// \return VertexCube
//-----------------------------------------------------------------------------
VertexCube CubeIdentify(
    /*[in]*/ const float ratio);