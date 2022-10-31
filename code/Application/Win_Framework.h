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

#include "../Utilities/Utilities.h"

using Microsoft::WRL::ComPtr;

#define SCREEN_MULTI    80                      /// ウィンドウ倍率
#define	SCREEN_WIDTH	(16 * SCREEN_MULTI)     /// ウインドウの幅
#define	SCREEN_HEIGHT	( 9 * SCREEN_MULTI)     /// ウインドウの高さ

#define FLAME_LATE      60
