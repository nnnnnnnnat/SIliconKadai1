//==============================================================================
/// Filename: Game_Input.cpp
/// Description: キーボードの情報を扱うクラス
/// Copyright (C) Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#include "Game_Input.h"

BYTE GameInput::mKeyState[256];
BYTE GameInput::mOldKeyState[256];

void GameInput::Init() {

}

void GameInput::Update() {
    // 前のフレームのキー状態を保存
    memcpy_s(mOldKeyState , sizeof(mOldKeyState) , mKeyState , sizeof(mKeyState));

    bool sts = GetKeyboardState(mKeyState);
}

bool GameInput::GetKeyPress(int vkey) {
    return mKeyState[vkey] & 0x80;
}

bool GameInput::GetKeyTrigger(int vkey) {
    return ( mKeyState[vkey] & 0x80 ) != 0 && ( mOldKeyState[vkey] & 0x80 ) == 0;
}

bool GameInput::GetKeyRelease(int vkey) {
    return ( mKeyState[vkey] & 0x80 ) == 0 && ( mOldKeyState[vkey] & 0x80 ) != 0;
}
