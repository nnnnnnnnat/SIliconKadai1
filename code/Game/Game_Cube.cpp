#include "Game_Cube.h"

#include "Game_Input.h"

#include "../Math/Math_Quartanion.h"

using namespace DirectX;

void GameCube::InitCube() {
    InitCube(1.0f);
}

void GameCube::InitCube(float _ratio) {
    m_cubeVertex = CubeIdentify(_ratio);
    XMMATRIX mat = XMMatrixIdentity();
    XMStoreFloat4x4(&m_mtx , mat);
    ResetAngle();
    InitTextureColor();
}

const VertexCube GameCube::GetCubeVertex() {
    return m_cubeVertex;
}

const DirectX::XMFLOAT4X4 GameCube::GetMatrix() {
    return m_mtx;
}

void GameCube::RotateMatrix() {
    DirectX::XMFLOAT3 angle = { 0.0f , 0.0f , 0.0f };
    ResetAngle();
    bool keyflg = false;
    if (GameInput::GetKeyPress(VK_UP)) {
        angle.x = 9.0f;
        m_angle[0] += 9.0f;
        keyflg = true;
    }
    if (GameInput::GetKeyPress(VK_DOWN)) {
        angle.x = -9.0f;
        m_angle[0] -= 9.0f;
        keyflg = true;
    }
    if (GameInput::GetKeyPress(VK_RIGHT)) {
        angle.y = -9.0f;
        m_angle[1] -= 9.0f;
        keyflg = true;
    }
    if (GameInput::GetKeyPress(VK_LEFT)) {
        angle.y = 9.0f;
        m_angle[1] += 9.0f;
        keyflg = true;
    }

    if (keyflg) {
        Quartanion::Calcurate(m_mtx , angle);
    }
}

//-----------------------------------------------------------------------------
/// ‰ñ“] Žæ“¾
/// 
/// \param [in] _axis Žæ“¾‚·‚éŽ² 0‚È‚çXŽ² 1‚È‚çYŽ² 2‚È‚çZŽ²
/// 
/// \return float
//-----------------------------------------------------------------------------
float GameCube::GetRotateAngle(int _axis) {
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

unsigned char GameCube::GetColorMap(int _x , int _y , int _color) {
    if (_color >= 0 && _color <= 2) {
        return m_textureColor[_y][_x][_color];
    }
    return 0;
}

void GameCube::ResetAngle() {
    m_angle[0] = 0;
    m_angle[1] = 0;
    m_angle[2] = 0;
}

void GameCube::InitTextureColor() {

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
            }
            else { // else
                m_textureColor[y][x][0] = 255;
                m_textureColor[y][x][1] = 255;
                m_textureColor[y][x][2] = 255;
            }
        }
    }

    int a;
}
