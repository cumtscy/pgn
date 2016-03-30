#pragma once
#include "../Common/Interface.h"
#include "CmpFunc.h"
namespace pgn {

enum MinFilter
{
	MIN_POINT,
	MIN_POINT_MIP_POINT,
	MIN_POINT_MIP_LINEAR,
	MIN_LINEAR_MIP_POINT,
	MIN_LINEAR_MIP_LINEAR
};

enum MagFilter
{
	MAG_POINT, MAG_LINEAR
};

enum UVWrap
{
	CLAMP, REPEAT, MIRROR
};

struct SamplerDesc
{
	MinFilter	minFilter;
	MagFilter	magFilter;
	UVWrap		wrapU;
	UVWrap		wrapV;
	float		maxAniso;
	CmpFunc		cmpFunc;
};

class Sampler : public Interface
{
public:
};

}
