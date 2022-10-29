//==============================================================================
/// Filename: Game_Input.cpp
/// Description: �L�[�{�[�h�̏��������N���X
/// Copyright (C) Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#include "Game_Input.h"

GameInput& GameInput::GetInstance() {
    static GameInput instance;
    return instance;
}

bool GameInput::Init() {

}

void GameInput::Update() {
    // �O�̃t���[���̃L�[��Ԃ�ۑ�
    memcpy_s(mOldKeyState , sizeof(mOldKeyState) , mKeyState , sizeof(mKeyState));

    bool sts = GetKeyboardState(mKeyState);
}

bool GameInput::GetKeyPress(const int _vkey) {
    return mKeyState[_vkey] & 0x80;
}

bool GameInput::GetKeyTrigger(const int _vkey) {
    return ( mKeyState[_vkey] & 0x80 ) != 0 && ( mOldKeyState[_vkey] & 0x80 ) == 0;
}

bool GameInput::GetKeyRelease(const int _vkey) {
    return ( mKeyState[_vkey] & 0x80 ) == 0 && ( mOldKeyState[_vkey] & 0x80 ) != 0;
}
