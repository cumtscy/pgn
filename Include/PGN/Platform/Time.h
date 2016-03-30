#pragma once
#include "../Common/DllInterface.h"
namespace pgn {

PGN_DLL_INTERFACE void getTimeUnit(long long* numer, long long* denom);
PGN_DLL_INTERFACE long long getTime();

}
