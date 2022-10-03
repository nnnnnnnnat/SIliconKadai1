#pragma once

#include <Windows.h>

namespace Util {
	void MessageBox_OK(const char* _text , const char* _title) {
		MessageBox(nullptr , _text , _title , MB_OK);
	}
}