//==============================================================================
/// Filename: Math_Quartanion.h
/// Description: �N�I�[�^�j�I���̌v�Z
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once 

#include <DirectXMath.h>
#include "../../framework.h"

class Quartanion {
public:
    //==============================================================================
    // public method

    //==============================================================================
    /// �s��̉�]���N�H�[�^�j�I���ōs��
    /// 
    /// \param  [out]   _mtx    ��]���������s��
    /// \param  [in]    _angle  �R���̉�]�p
    /// 
    /// \return void 
    //==============================================================================
    static void Calcurate(
        /*[out]*/   DirectX::XMFLOAT4X4& _mtx ,
        /*[in]*/    const DirectX::XMFLOAT3& _angle);

    //==============================================================================

private:
    //==============================================================================
    /// �s�񂩂�N�H�[�^�j�I�����쐬
    /// 
    /// \param  [in]    _mtx    �s��
    /// \param  [out]   _qt     �N�I�[�^�j�I��
    /// 
    /// return void
    //==============================================================================
    static void GetQuartanionFromMatrix(
        /*[in]*/    const DirectX::XMFLOAT4X4& _mtx ,
        /*[out]*/   DirectX::XMFLOAT4& _qt);

    //==============================================================================
    /// �N�H�[�^�j�I������s����쐬
    /// 
    /// \param  [in]    _mtx    �s��
    /// \param  [out]   _qt     �N�I�[�^�j�I��
    /// 
    /// return void
    //==============================================================================
    static void GetMatrixFromQuartanion(
        /*[out]*/   DirectX::XMFLOAT4X4& _mtx ,
        /*[in]*/    const DirectX::XMFLOAT4& _qt);

    //==============================================================================
    /// �w�莲�ɉ�]�����N�H�[�^�j�I�����쐬
    /// 
    /// \param  [out]   _qt     �N�I�[�^�j�I��
    /// \param  [in]    _axis   ��]�p
    /// \param  [in]    _angle  �p�x
    /// 
    /// return void
    //==============================================================================
    static void RotationAxis(
        /*[out]*/   DirectX::XMFLOAT4& _qt ,
        /*[in]*/    const DirectX::XMFLOAT4& _axis ,
        /*[in]*/    const float _angle);

    //==============================================================================
    /// �N�H�[�^�j�I���̏�Z
    /// 
    /// \param  [out]   _outqt  ��
    /// \param  [in]    _qt1    �����鐔�P
    /// \param  [in]    _qt2    �����鐔�Q
    /// 
    /// return void
    //==============================================================================
    static void Multiplication(
        /*[out]*/   DirectX::XMFLOAT4& _outqt ,
        /*[in]*/    const DirectX::XMFLOAT4& _qt1 ,
        /*[in]*/    const DirectX::XMFLOAT4& _qt2);

    //==============================================================================
    /// �N�H�[�^�j�I���̂̐��K��
    /// 
    /// \param  [in]    _inqt   ���K���O
    /// \param  [out]   _outqt  ���K����
    /// 
    /// return void
    //==============================================================================    
    static void Normalise(
        /*[in]*/    const DirectX::XMFLOAT4& _inqt ,
        /*[out]*/   DirectX::XMFLOAT4& _outqt);

};


