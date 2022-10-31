//==============================================================================
/// Filename: Utilities.cpp
/// Description: 共通のメソッド
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#include "Utilities.h"

template<typename T> void SafeRelease(T*& ptr) {
    if (ptr != nullptr) {
        ptr->Release();
        ptr = nullptr;
    }
}

VertexCube CubeIdentify() {
    return CubeIdentify(1.0f);
}

VertexCube CubeIdentify(const float ratio) {

    VertexCube cube;

    // 辺の長さ(中心座標は立方体の中心に設定)
    float length = ratio / 2.0f;

    // 色設定(白)
    DirectX::XMFLOAT4 color = { 1.0f , 1.0f , 1.0f , 1.0f };

    // uv座標格納
    std::array<DirectX::XMFLOAT2 , 4> uc;
    uc[0] = { 0.0f , 0.0f };
    uc[1] = { 1.0f , 0.0f };
    uc[2] = { 0.0f , 1.0f };
    uc[3] = { 1.0f , 1.0f };

    // 頂点に情報を格納していく
    cube[0] = { { -length , +length , -length } , color , uc[0] };
    cube[1] = { { +length , +length , -length } , color , uc[1] };
    cube[2] = { { +length , -length , -length } , color , uc[3] };
    cube[3] = { { +length , -length , -length } , color , uc[3] };
    cube[4] = { { -length , -length , -length } , color , uc[2] };
    cube[5] = { { -length , +length , -length } , color , uc[0] };

    cube[6] = { { +length , +length , +length } , color , uc[1] };
    cube[7] = { { -length , +length , +length } , color , uc[0] };
    cube[8] = { { -length , -length , +length } , color , uc[2] };
    cube[9] = { { -length , -length , +length } , color , uc[2] };
    cube[10] = { { +length , -length , +length } , color , uc[3] };
    cube[11] = { { +length , +length , +length } , color , uc[1] };

    cube[12] = { { +length , -length , -length } , color , uc[0] };
    cube[13] = { { +length , +length , -length } , color , uc[1] };
    cube[14] = { { +length , +length , +length } , color , uc[3] };
    cube[15] = { { +length , +length , +length } , color , uc[3] };
    cube[16] = { { +length , -length , +length } , color , uc[2] };
    cube[17] = { { +length , -length , -length } , color , uc[0] };

    cube[18] = { { -length , -length , +length } , color , uc[0] };
    cube[19] = { { -length , +length , +length } , color , uc[1] };
    cube[20] = { { -length , +length , -length } , color , uc[3] };
    cube[21] = { { -length , +length , -length } , color , uc[3] };
    cube[22] = { { -length , -length , -length } , color , uc[2] };
    cube[23] = { { -length , -length , +length } , color , uc[0] };

    cube[24] = { { -length , +length , +length } , color , uc[0] };
    cube[25] = { { +length , +length , +length } , color , uc[1] };
    cube[26] = { { +length , +length , -length } , color , uc[3] };
    cube[27] = { { +length , +length , -length } , color , uc[3] };
    cube[28] = { { -length , +length , -length } , color , uc[2] };
    cube[29] = { { -length , +length , +length } , color , uc[0] };

    cube[30] = { { +length , -length , -length } , color , uc[0] };
    cube[31] = { { +length , -length , +length } , color , uc[1] };
    cube[32] = { { -length , -length , +length } , color , uc[3] };
    cube[33] = { { -length , -length , +length } , color , uc[3] };
    cube[34] = { { -length , -length , -length } , color , uc[2] };
    cube[35] = { { +length , -length , -length } , color , uc[0] };

    return cube;
}
