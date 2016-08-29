#pragma once
#include <PGN/Math/Math.h>
namespace pgn {

PGN_DLL_INTERFACE bool testSphereSphere(Float2* c1, float r1, Float2* c2, float r2, Float2* sepVec);
PGN_DLL_INTERFACE bool testCapsuleCapsule(Float2 ab1[], float r1, Float2 ab2[], float r2, Float2* sepVec);

}
