//==============================================================================
/// Filename: GameCube.cpp
/// Description: キューブの抽象化クラス
/// Copyright (C) Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#include "Game_Object.h"

#include "Game_Input.h"

#include "../DX11/DX11_Object.h"
#include "../DX12/DX12_Object.h"
#include "../OpenGL/OpenGL_Object.h"
#include "../Math/Math_Quartanion.h"

using namespace DirectX;

GameObject* GameObject::GetObjectPtr(GameLayerMgr::RendererType _type) {
    GameObject* _obj = nullptr;

    switch (_type) { // switch 
    case GameLayerMgr::RendererType::DirectX11:
        _obj = new DX11Object();
        break;
    case GameLayerMgr::RendererType::DirectX12:
        _obj = new DX12Object();
        break;
    case GameLayerMgr::RendererType::OpenGL:
        _obj = new OpenGLObject();
        break;
    }

    return _obj;
}

void GameObject::InitCube() {
    InitCube(1.0f);
}

void GameObject::InitCube(float _ratio) {
    m_cubeVertex = CubeIdentify(_ratio);
    XMMATRIX mat = XMMatrixIdentity();
    XMStoreFloat4x4(&m_mtx , mat);
    ResetAngle();
    InitTextureColor();
}

const VertexCube GameObject::GetCubeVertex() {
    return m_cubeVertex;
}

const DirectX::XMFLOAT4X4 GameObject::GetMatrix() {
    return m_mtx;
}

void GameObject::RotateMatrix() {
    DirectX::XMFLOAT3 angle = { 0.0f , 0.0f , 0.0f };
    ResetAngle();
    bool keyflg = false;
    if (GameInput::GetInstance().GetKeyPress(VK_UP)) {
        angle.x = 9.0f;
        m_angle[0] += 9.0f;
        keyflg = true;
    }
    if (GameInput::GetInstance().GetKeyPress(VK_DOWN)) {
        angle.x = -9.0f;
        m_angle[0] -= 9.0f;
        keyflg = true;
    }
    if (GameInput::GetInstance().GetKeyPress(VK_RIGHT)) {
        angle.y = -9.0f;
        m_angle[1] -= 9.0f;
        keyflg = true;
    }
    if (GameInput::GetInstance().GetKeyPress(VK_LEFT)) {
        angle.y = 9.0f;
        m_angle[1] += 9.0f;
        keyflg = true;
    }

    if (keyflg) {
        Quartanion::Calcurate(m_mtx , angle);
    }
}

float GameObject::GetRotateAngle(int _axis) {
    switch (_axis) { // switch
    case 0: // X
        return m_angle[0];
        break;
    case 1: // Y
        return m_angle[1];
        break;
    case 2: // Z
        return m_angle[2];
        break;
    }
    return 0.0f;
}

unsigned char GameObject::GetTexturePixelColor(int _x , int _y , int _color) {
    if (_color >= 0 && _color <= 3) {
        return m_textureColor[_y][_x][_color];
    }
    return 0;
}

void GameObject::ResetAngle() {
    m_angle[0] = 0;
    m_angle[1] = 0;
    m_angle[2] = 0;
}

void GameObject::InitTextureColor() {

    // 日の丸のテクスチャを作る

    const int table[32][2] = {
        { -1 , -1 } ,
        { -1 , -1 } ,
        { -1 , -1 } ,
        { -1 , -1 } ,
        { -1 , -1 } ,
        { 14 , 17 } ,
        { 11 , 20 } ,
        { 9 , 22 } ,
        { 8 , 23 } ,
        { 7 , 24 } ,
        { 7 , 24 } ,
        { 6 , 25 } ,
        { 6 , 25 } ,
        { 6 , 25 } ,
        { 5 , 26 } ,
        { 5 , 26 } ,
        { 5 , 26 } ,
        { 5 , 26 } ,
        { 6 , 25 } ,
        { 6 , 25 } ,
        { 6 , 25 } ,
        { 7 , 24 } ,
        { 7 , 24 } ,
        { 8 , 23 } ,
        { 9 , 22 } ,
        { 11 , 20 } ,
        { 14 , 17 } ,
        { -1 , -1 } ,
        { -1 , -1 } ,
        { -1 , -1 } ,
        { -1 , -1 } ,
        { -1 , -1 }
    };

    for (int y = 0; y < TEXTURE_SIZE_Y; y++) { // for
        for (int x = 0; x < TEXTURE_SIZE_Y; x++) { // for
            if (x >= table[y][0] && x <= table[y][1]) { // if
                m_textureColor[y][x][0] = 255;
                m_textureColor[y][x][1] = 0;
                m_textureColor[y][x][2] = 0;
                m_textureColor[y][x][3] = 255;
            }
            else { // else
                m_textureColor[y][x][0] = 255;
                m_textureColor[y][x][1] = 255;
                m_textureColor[y][x][2] = 255;
                m_textureColor[y][x][3] = 255;
            }
        }
    }
}
