#pragma once
#include "../Common/aligned.h"
#include "../Common/DllInterface.h"
namespace pgn {

struct Float2
{
	_aligned(8, float v[2]);

	float& operator[](int i)
	{
		return v[i];
	}
};

struct Float3
{
	_aligned(16, float v[3]);

	float& operator[](int i)
	{
		return v[i];
	}
};

struct Float4
{
	union
	{
		_aligned(16, float v[4]);
		Float3 float3;
	};

	float& operator[](int i)
	{
		return v[i];
	}

	void operator=(Float3& v)
	{
		float3 = v;
	}
};

struct Float4x3
{
	Float4 col[3];

	Float4& operator[](int i)
	{
		return col[i];
	}
};

struct Float4x4
{
	union
	{
		Float4 col[4];
		Float4x3 float4x3;
	};

	Float4& operator[](int i)
	{
		return col[i];
	}

	void operator=(Float4x3& mat)
	{
		float4x3 = mat;
	}
};

struct Float3x2
{
	Float3 col[2];

	Float3& operator[](int i)
	{
		return col[i];
	}
};

PGN_DLL_INTERFACE void mul(Float4x4* result, Float4x4* a, Float4x4* b);
PGN_DLL_INTERFACE void mul(Float4x3* result, Float4x4* a, Float4x3* b);
PGN_DLL_INTERFACE void mul(Float4x3* result, Float4x3* a, Float4x3* b); // 3x3æÿ’Û≥À∑®

PGN_DLL_INTERFACE void lerp(Float4* result, Float4* a, Float4* b, float t);
PGN_DLL_INTERFACE void slerp(Float4* result, Float4* a, Float4* b, float t);

PGN_DLL_INTERFACE void quatToMat(Float4x3* m, Float4* q);

PGN_DLL_INTERFACE void add(Float4* results, Float4* a, Float4* b, int count);
PGN_DLL_INTERFACE void sub(Float4* results, Float4* a, Float4* b, int count);

}
