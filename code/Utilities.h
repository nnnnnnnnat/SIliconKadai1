//==============================================================================
// Filename: Utilities.h
// Description: 
// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================
#pragma once

#include <Windows.h>

// 16�o�C�g�ɃA���C�������g����B
#define ALIGN16 _declspec(align(16))

namespace Util {

    class NonCopyable {
    protected:
        NonCopyable() {}
        ~NonCopyable() {}
    private:
        NonCopyable& operator = (const NonCopyable& src) {};
        NonCopyable(const NonCopyable& src) {};
    };

    // ���b�Z�[�W����
    inline void MessageBox_OK(const char* _text , const char* _title) {
        MessageBoxA(nullptr , _text , _title , MB_OK);
    }
}