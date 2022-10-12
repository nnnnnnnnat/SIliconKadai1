// header.h : 標準のシステム インクルード ファイルのインクルード ファイル、
// またはプロジェクト専用のインクルード ファイル


#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
// Windows ヘッダー ファイル
#include <windows.h>
// C ランタイム ヘッダー ファイル
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <array>
#include <vector>
#include <string>

#include "code/Utilities.h"

using Microsoft::WRL::ComPtr;

struct Vertex {
    DirectX::XMFLOAT3 m_pos;	// 座標
    DirectX::XMFLOAT4 m_color;
    DirectX::XMFLOAT2 m_tex;
};

struct ConstantBuffer {
    DirectX::XMFLOAT4X4 m_world;
    DirectX::XMFLOAT4X4 m_view;
    DirectX::XMFLOAT4X4 m_projection;
};
