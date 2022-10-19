#pragma once

#include <Windows.h>

class GameInput {
public:
    static void Init();
    static void Update();
    static bool GetKeyPress(int);
    static bool GetKeyTrigger(int);
    static bool GetKeyRelease(int);
private:
    static BYTE mKeyState[256];
    static BYTE mOldKeyState[256];
};

