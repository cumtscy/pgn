#define PGN_DLL_EXPORT
#include <PGN/Math/Math.h>
#undef PGN_DLL_EXPORT

#include <math.h>

void pgn::mul(Float4x4* _result, Float4x4* _a, Float4x4* _b)
{
	Float4x4& r = *_result;
	Float4x4& a = *_a;
	Float4x4& b = *_b;

	for(int row = 0; row < 4; row++)
		for(int col = 0; col < 4; col++)
			r[col][row] = a[0][row] * b[col][0] + a[1][row] * b[col][1] + a[2][row] * b[col][2] + a[3][row] * b[col][3];
}

void pgn::mul(Float4x3* _result, Float4x4* _a, Float4x3* _b)
{
	Float4x3& r = *_result;
	Float4x4& a = *_a;
	Float4x3& b = *_b;

	for (int row = 0; row < 4; row++)
	for (int col = 0; col < 3; col++)
		r[col][row] = a[0][row] * b[col][0] + a[1][row] * b[col][1] + a[2][row] * b[col][2] + a[3][row] * b[col][3];
}

void pgn::mul(Float4x3* _result, Float4x3* _a, Float4x3* _b)
{
	Float4x3& r = *_result;
	Float4x3& a = *_a;
	Float4x3& b = *_b;

	for(int row = 0; row < 3; row++)
		for(int col = 0; col < 3; col++)
			r[col][row] = a[0][row] * b[col][0] + a[1][row] * b[col][1] + a[2][row] * b[col][2];
}

void pgn::lerp(Float4* _result, Float4* _a, Float4* _b, float t)
{
	Float4& r = *_result;
	Float4& a = *_a;
	Float4& b = *_b;

	float k0 = 1.0f - t;
	float k1 = t;
	r[0] = a[0] * k0 + b[0] * k1;
	r[1] = a[1] * k0 + b[1] * k1;
	r[2] = a[2] * k0 + b[2] * k1;
}

void pgn::slerp(Float4* _result, Float4* _a, Float4* _b, float t)
{
	Float4& r = *_result;
	Float4& a = *_a;
	Float4& b = *_b;

	float cosOmega = a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];

	if (cosOmega < 0.0f)
	{
		b[0] = -b[0];
		b[1] = -b[1];
		b[2] = -b[2];
		b[3] = -b[3];
		cosOmega = -cosOmega;
	}

	float k0, k1;

	if (cosOmega > 0.9999f)
	{
		k0 = 1.0f - t;
		k1 = t;
	}
	else
	{
		float sinOmega = sqrt(1.0f - cosOmega * cosOmega);
		float omega = atan2(sinOmega, cosOmega);
		float oneOverSinOmega = 1.0f / sinOmega;
		k0 = sin((1.0f - t) * omega) * oneOverSinOmega;
		k1 = sin(t * omega) * oneOverSinOmega;
	}

	r[0] = a[0] * k0 + b[0] * k1;
	r[1] = a[1] * k0 + b[1] * k1;
	r[2] = a[2] * k0 + b[2] * k1;
	r[3] = a[3] * k0 + b[3] * k1;
}

void pgn::quatToMat(Float4x3* _m, Float4* _q)
{
	Float4x3& m = *_m;
	Float4& q = *_q;

	float _2xx = 2.0f * q[0] * q[0];
	float _2xy = 2.0f * q[0] * q[1];
	float _2xz = 2.0f * q[0] * q[2];
	float _2xw = 2.0f * q[0] * q[3];
	float _2yy = 2.0f * q[1] * q[1];
	float _2yz = 2.0f * q[1] * q[2];
	float _2yw = 2.0f * q[1] * q[3];
	float _2zz = 2.0f * q[2] * q[2];
	float _2zw = 2.0f * q[2] * q[3];

	m[0][0] = 1.0f - _2yy - _2zz;	m[1][0] = _2xy + _2zw;			m[2][0] = _2xz - _2yw;
	m[0][1] = _2xy - _2zw;			m[1][1] = 1.0f - _2xx - _2zz;	m[2][1] = _2yz + _2xw;
	m[0][2] = _2xz + _2yw;			m[1][2] = _2yz - _2xw;			m[2][2] = 1.0f - _2xx - _2yy;
}

void pgn::add(Float4* results, Float4* a, Float4* b, int count)
{
	for (int i = 0; i < count; i++)
	{
		results[i][0] = a[i][0] + b[i][0];
		results[i][1] = a[i][1] + b[i][1];
		results[i][2] = a[i][2] + b[i][2];
		results[i][3] = a[i][3] + b[i][3];
	}
}

void pgn::sub(Float4* results, Float4* a, Float4* b, int count)
{
	for (int i = 0; i < count; i++)
	{
		results[i][0] = a[i][0] - b[i][0];
		results[i][1] = a[i][1] - b[i][1];
		results[i][2] = a[i][2] - b[i][2];
		results[i][3] = a[i][3] - b[i][3];
	}
}
