#include "Utilities.h"

template<typename T> void SafeRelease(T*& _ptr) {
    if (_ptr != nullptr) {
        _ptr->Release();
        _ptr = nullptr;
    }
}
