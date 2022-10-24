#include "Game_Cube.h"

#include "Game_Input.h"

#include "../Math/Math_Quartanion.h"

using namespace DirectX;

bool GameCube::Init() {
    Init(1.0f);
    return true;
}

bool GameCube::Init(const float ratio) {
    m_cubeVertex = CubeIdentify(ratio);
    XMMATRIX mat = XMMatrixIdentity();
    XMStoreFloat4x4(&m_mtx , mat);
    return true;
}

void GameCube::Update() {
    // YŽ²‰ñ“]‚³‚¹‚é
    DirectX::XMFLOAT3 angle = { 0.0f , 0.0f , 0.0f };
    bool keyflg = false;
    if (GameInput::GetKeyPress(VK_UP)) {
        angle.x = 3.0f;
        keyflg = true;
    }
    if (GameInput::GetKeyPress(VK_DOWN)) {
        angle.x = -3.0f;
        keyflg = true;
    }
    if (GameInput::GetKeyPress(VK_RIGHT)) {
        angle.y = -3.0f;
        keyflg = true;
    }
    if (GameInput::GetKeyPress(VK_LEFT)) {
        angle.y = 3.0f;
        keyflg = true;
    }

    if (keyflg) {
        Quartanion::Calcurate(m_mtx , angle);
    }

}

VertexCube GameCube::GetCubevertex() {
    return m_cubeVertex;
}

DirectX::XMFLOAT4X4 GameCube::GetMatrix() {
    return m_mtx;
}
