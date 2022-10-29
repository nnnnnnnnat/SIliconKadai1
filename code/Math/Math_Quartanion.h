//==============================================================================
/// Filename: Math_Quartanion.h
/// Description: クオータニオンの計算
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
    /// 行列の回転をクォータニオンで行う
    /// 
    /// \param  [out]   _mtx    回転させたい行列
    /// \param  [in]    _angle  ３軸の回転角
    /// 
    /// \return void 
    //==============================================================================
    static void Calcurate(
        /*[out]*/   DirectX::XMFLOAT4X4& _mtx ,
        /*[in]*/    const DirectX::XMFLOAT3& _angle);

    //==============================================================================

private:
    //==============================================================================
    /// 行列からクォータニオンを作成
    /// 
    /// \param  [in]    _mtx    行列
    /// \param  [out]   _qt     クオータニオン
    /// 
    /// return void
    //==============================================================================
    static void GetQuartanionFromMatrix(
        /*[in]*/    const DirectX::XMFLOAT4X4& _mtx ,
        /*[out]*/   DirectX::XMFLOAT4& _qt);

    //==============================================================================
    /// クォータニオンから行列を作成
    /// 
    /// \param  [in]    _mtx    行列
    /// \param  [out]   _qt     クオータニオン
    /// 
    /// return void
    //==============================================================================
    static void GetMatrixFromQuartanion(
        /*[out]*/   DirectX::XMFLOAT4X4& _mtx ,
        /*[in]*/    const DirectX::XMFLOAT4& _qt);

    //==============================================================================
    /// 指定軸に回転したクォータニオンを作成
    /// 
    /// \param  [out]   _qt     クオータニオン
    /// \param  [in]    _axis   回転角
    /// \param  [in]    _angle  角度
    /// 
    /// return void
    //==============================================================================
    static void RotationAxis(
        /*[out]*/   DirectX::XMFLOAT4& _qt ,
        /*[in]*/    const DirectX::XMFLOAT4& _axis ,
        /*[in]*/    const float _angle);

    //==============================================================================
    /// クォータニオンの乗算
    /// 
    /// \param  [out]   _outqt  積
    /// \param  [in]    _qt1    かける数１
    /// \param  [in]    _qt2    かける数２
    /// 
    /// return void
    //==============================================================================
    static void Multiplication(
        /*[out]*/   DirectX::XMFLOAT4& _outqt ,
        /*[in]*/    const DirectX::XMFLOAT4& _qt1 ,
        /*[in]*/    const DirectX::XMFLOAT4& _qt2);

    //==============================================================================
    /// クォータニオンのの正規化
    /// 
    /// \param  [in]    _inqt   正規化前
    /// \param  [out]   _outqt  正規化後
    /// 
    /// return void
    //==============================================================================    
    static void Normalise(
        /*[in]*/    const DirectX::XMFLOAT4& _inqt ,
        /*[out]*/   DirectX::XMFLOAT4& _outqt);

};


