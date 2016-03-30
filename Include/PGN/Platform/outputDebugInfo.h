#pragma once
#include "../Common/DllInterface.h"

#ifdef NDEBUG
#define _outputDebugInfo(str) ((void)0)
#else
#define _outputDebugInfo(str) pgn::outputDebugInfo((char*)str);
#endif

namespace pgn {

PGN_DLL_INTERFACE void outputDebugInfo(char* str);

}
