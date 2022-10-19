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
    /// _mtx [out] ��]���������s��
    /// _angle [in] �R���̉�]�p
    /// 
    /// \return void 
    //==============================================================================
    static void Calcurate(
        /*[out]*/ DirectX::XMFLOAT4X4& _mtx ,
        /*[in]*/const DirectX::XMFLOAT3& _angle);

    //==============================================================================

private:
    //==============================================================================
    /// �s�񂩂�N�H�[�^�j�I�����쐬
    /// 
    /// _mtx [out] �s��
    /// _qt [in] �N�I�[�^�j�I��
    /// 
    /// return void
    //==============================================================================
    static void GetQuartanionFromMatrix(
        /*[out]*/ const DirectX::XMFLOAT4X4& _mtx ,
        /*[in]*/ DirectX::XMFLOAT4& _qt);

    //==============================================================================
    /// �N�H�[�^�j�I������s����쐬
    /// 
    /// _mtx [in] �s��
    /// _qt [out] �N�I�[�^�j�I��
    /// 
    /// return void
    //==============================================================================
    static void GetMatrixFromQuartanion(
        /*[out]*/ DirectX::XMFLOAT4X4& _mtx ,
        /*[]in*/ const DirectX::XMFLOAT4& _qt);

    //==============================================================================
    /// �w�莲�ɉ�]�����N�H�[�^�j�I�����쐬
    /// 
    /// _qt [out] �N�I�[�^�j�I��
    /// _axis [in] ��]�p
    /// _angle [in] �p�x
    /// 
    /// return void
    //==============================================================================
    static void RotationAxis(
        /*[out]*/ DirectX::XMFLOAT4& _qt ,
        /*[in]*/ const DirectX::XMFLOAT4& _axis ,
        /*[in]*/ const float _angle);

    //==============================================================================
    /// �N�H�[�^�j�I���̏�Z
    /// 
    /// _outqt [out] ��
    /// _qt1 [in] �����鐔�P
    /// _qt2 [in] �����鐔�Q
    /// 
    /// return void
    //==============================================================================
    static void Multiplication(
        /*[out]*/ DirectX::XMFLOAT4& _outqt ,
        /*[in]*/ const DirectX::XMFLOAT4& _qt1 ,
        /*[in]*/ const DirectX::XMFLOAT4& _qt2);

    //==============================================================================
    /// �N�H�[�^�j�I���̂̐��K��
    /// 
    /// _inqt [in] ���K���O
    /// _outqt [out] ���K����
    /// 
    /// return void
    //==============================================================================    
    static void Normalise(
        /*[in]*/ const DirectX::XMFLOAT4& _inqt ,
        /*[out]*/ DirectX::XMFLOAT4& _outqt);

};


