//==============================================================================
// Filename: Utilities.h
// Description: 
// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================
#pragma once

#include <Windows.h>

namespace Util {
	void MessageBox_OK(const char* _text , const char* _title) {
		MessageBoxA(nullptr , _text , _title , MB_OK);
	}
}