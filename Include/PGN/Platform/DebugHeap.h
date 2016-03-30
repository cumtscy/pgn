#pragma once
#include "../Common/DllInterface.h"

#ifdef NDEBUG
#define _beginDebugHeap() ((void)0)
#define _endDebugHeap() ((void)0)
#else
#define _beginDebugHeap() pgn::beginDebugHeap();
#define _endDebugHeap() pgn::endDebugHeap();
#endif

namespace pgn {

PGN_DLL_INTERFACE void beginDebugHeap();
PGN_DLL_INTERFACE void endDebugHeap();

}
