#pragma once
#include "../Common/Interface.h"
namespace pgn {

enum VertexAttribFormat // GeometryHelper::createGeometry sizeOf
{
	FLOAT, FLOAT2, FLOAT3, FLOAT4,
	HALF, HALF2, HALF3, HALF4,
	BYTE, BYTE2, BYTE3, BYTE4,
	UBYTE, UBYTE2, UBYTE3, UBYTE4,
	SHORT, SHORT2, SHORT3, SHORT4,
	USHORT, USHORT2, USHORT3, USHORT4,
};

struct VertexAttribDesc
{
	const char*			name;
	VertexAttribFormat	format;
	int					instanceDataStepRate;
};

class VertexInputLayout : public Interface
{
public:
};

}
