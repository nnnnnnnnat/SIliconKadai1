#include "Game_Cube.h"

void GameCube::Init() {
    Init(1.0f);
}

void GameCube::Init(const float ratio) {
    m_cube = CubeIdentify(ratio);
}

VertexCube GameCube::GetCubeParamator() const {
    return m_cube;
}
