#include "Game_Cube.h"

#include "../Math/Math_Quartanion.h"

bool GameCube::Init() {
    Init(1.0f);
    return true;
}

bool GameCube::Init(const float ratio) {
    m_cubeVertex = CubeIdentify(ratio);
    return true;
}

void GameCube::Update() {
    // YŽ²‰ñ“]‚³‚¹‚é
    const DirectX::XMFLOAT3 angle = { 0.0f , 0.025f , 0.0f };
    Quartanion::Calcurate(m_mtx , angle);
}

VertexCube GameCube::GetCubevertex() {
    return m_cubeVertex;
}

DirectX::XMFLOAT4X4 GameCube::GetMatrix() {
    return m_mtx;
}
