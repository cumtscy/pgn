#define PGN_DLL_EXPORT
#include <PGN/FatMan/LinearTransformations.h>
#undef PGN_DLL_EXPORT

#include "sinCosTable.h"

void pgn::computeWorldMat(pgn::Float4x3* result, float x, float y, float z, int yaw)
{
	pgn::Float4x3& m = *result;

	pgn::Float2 sinCos = sinCosTable[yaw];

	m[0][0] = sinCos[1];	m[1][0] = 0;		m[2][0] = -sinCos[0];
	m[0][1] = 0;			m[1][1] = 1;		m[2][1] = 0;
	m[0][2] = sinCos[0];	m[1][2] = 0;		m[2][2] = sinCos[1];
	m[0][3] = x;			m[1][3] = y;		m[2][3] = z;
}
