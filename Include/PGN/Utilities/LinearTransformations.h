#pragma once
#include <PGN/Math/Math.h>
namespace pgn {

const int turnDiv = 512; // Ò»È¦512¶È

PGN_DLL_INTERFACE void computeWorldMat(pgn::Float4x3* result, float x, float y, float z, int yaw);

}
