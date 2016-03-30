#pragma once
#include <stddef.h>
#include "../Common/DllInterface.h"

#ifdef NDEBUG
#define _pageAlloc(size) pgn::pageAlloc(size)
#else
#define _pageAlloc(size) pgn::pageAlloc(size, __FILE__, __LINE__)
#endif

namespace pgn {

PGN_DLL_INTERFACE unsigned getPageSize();
PGN_DLL_INTERFACE void* pageAlloc(size_t size);
PGN_DLL_INTERFACE void* pageAlloc(size_t size, const char file[], int line);
PGN_DLL_INTERFACE void pageFree(void* p, size_t size);

}
