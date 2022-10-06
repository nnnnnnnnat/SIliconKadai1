//==============================================================================
// Filename: Utilities.h
// Description: 
// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================
#pragma once

#include <Windows.h>

// 16バイトにアラインメントする。
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

    // メッセージ入力
    inline void MessageBox_OK(const char* _text , const char* _title) {
        MessageBoxA(nullptr , _text , _title , MB_OK);
    }
}