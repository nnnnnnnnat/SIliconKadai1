//==============================================================================
/// Filename: testmain.cpp
/// Description: 
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#include "testmain.h"

#include "Utilities.h"

void MainLoop() {
    GameUpdate();
    GameDraw();
}

void GameInit() {
    Util::MessageBox_OK("test" , "init");
}

void GameUpdate() {
    Util::MessageBox_OK("test" , "update");
}

void GameDraw() {
    Util::MessageBox_OK("test" , "draw");
}