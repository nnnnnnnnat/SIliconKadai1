//==============================================================================
/// Filename: Math_Quartanion.cpp
/// Description: クオータニオンの計算
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#include "Math_Quartanion.h"

void Quartanion::Calcurate(DirectX::XMFLOAT4X4& _mtx , const DirectX::XMFLOAT3& _angle) {

    // それぞれの軸を格納する変数
    DirectX::XMFLOAT4 r_axisX;
    DirectX::XMFLOAT4 r_axisY;
    DirectX::XMFLOAT4 r_axisZ;

    // X軸成分
    r_axisX.x = _mtx._11;
    r_axisX.y = _mtx._12;
    r_axisX.z = _mtx._13;
    r_axisX.w = 0.0f;

    // Y軸成分
    r_axisY.x = _mtx._21;
    r_axisY.y = _mtx._22;
    r_axisY.z = _mtx._23;
    r_axisY.w = 0.0f;

    // Z軸成分
    r_axisZ.x = _mtx._31;
    r_axisZ.y = _mtx._32;
    r_axisZ.z = _mtx._33;
    r_axisZ.w = 0.0f;

    DirectX::XMFLOAT4 r_qt;

    // 行列からクオータニオン作成
    Quartanion::GetQuartanionFromMatrix(_mtx , r_qt);

    DirectX::XMFLOAT4 qtx , qty , qtz;
    DirectX::XMFLOAT4 tempqt1 , tempqt2 , tempqt3;

    Quartanion::RotationAxis(qtx , r_axisX , _angle.x);
    Quartanion::RotationAxis(qty , r_axisY , _angle.y);
    Quartanion::RotationAxis(qtz , r_axisZ , _angle.z);

    Quartanion::Multiplication(tempqt1 , r_qt , qtx);
    Quartanion::Multiplication(tempqt2 , qty , qtz);
    Quartanion::Multiplication(tempqt3 , tempqt1 , tempqt2);

    // 正規化
    Quartanion::Normalise(tempqt3 , tempqt3);

    // 行列を作成
    Quartanion::GetMatrixFromQuartanion(_mtx , tempqt3);

}

void Quartanion::GetQuartanionFromMatrix(const DirectX::XMFLOAT4X4& _mtx , DirectX::XMFLOAT4& _qt) {

    ALIGN16 DirectX::XMMATRIX r_mtx;
    ALIGN16 DirectX::XMVECTOR r_outvec;

    r_mtx = DirectX::XMLoadFloat4x4(&_mtx);

    r_outvec = DirectX::XMQuaternionRotationMatrix(r_mtx);

    DirectX::XMStoreFloat4(&_qt , r_outvec);

}

void Quartanion::GetMatrixFromQuartanion(DirectX::XMFLOAT4X4& _outmtx , const DirectX::XMFLOAT4& _qt) {

    ALIGN16 DirectX::XMMATRIX r_mtx;
    ALIGN16 DirectX::XMVECTOR r_qtvec;

    r_qtvec = DirectX::XMLoadFloat4(&_qt);

    r_mtx = DirectX::XMMatrixRotationQuaternion(r_qtvec);

    XMStoreFloat4x4(&_outmtx , r_mtx);

}

void Quartanion::RotationAxis(DirectX::XMFLOAT4& _qt , const DirectX::XMFLOAT4& _axis , const float _angle) {

    ALIGN16 DirectX::XMVECTOR outqt;
    ALIGN16 DirectX::XMVECTOR axisvec;

    axisvec = DirectX::XMLoadFloat4(&_axis);

    outqt = DirectX::XMQuaternionRotationAxis(axisvec , ( _angle * 3.1415926535f ) / 180.0f);

    DirectX::XMStoreFloat4(&_qt , outqt);

}

void Quartanion::Multiplication(DirectX::XMFLOAT4& _outqt , const DirectX::XMFLOAT4& _qt1 , const DirectX::XMFLOAT4& _qt2) {

    ALIGN16 DirectX::XMVECTOR qtvec1;
    ALIGN16 DirectX::XMVECTOR qtvec2;
    ALIGN16 DirectX::XMVECTOR outqtvec;

    qtvec1 = DirectX::XMLoadFloat4(&_qt1);
    qtvec2 = DirectX::XMLoadFloat4(&_qt2);

    outqtvec = DirectX::XMQuaternionMultiply(qtvec1 , qtvec2);

    DirectX::XMStoreFloat4(&_outqt , outqtvec);

}

void Quartanion::Normalise(const DirectX::XMFLOAT4& _inqt , DirectX::XMFLOAT4& _outqt) {

    ALIGN16 DirectX::XMVECTOR r_qtoutvec;
    ALIGN16 DirectX::XMVECTOR r_qtvec;

    r_qtvec = XMLoadFloat4(&_inqt);

    r_qtoutvec = DirectX::XMQuaternionNormalize(r_qtvec);

    XMStoreFloat4(&_outqt , r_qtoutvec);

}
