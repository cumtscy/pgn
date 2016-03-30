#pragma once
#include "../Common/Interface.h"
#include "CmpFunc.h"
namespace pgn {

enum StencilOp
{
	KEEP,
	CLEAR,
	REPLACE,
	INCR_SAT,
	DECR_SAT,
	INVERT,
	INCR_WRAP,
	DECR_WRAP
};

struct DepthStencilStateDesc
{
	bool			depthEnable;
	bool			depthWrite;
	CmpFunc			depthFunc;

	bool			stencilEnable;
	unsigned char	stencilReadMask;
	unsigned char	stencilWriteMask;

	struct {
		CmpFunc		stencilFunc;
		StencilOp	stencilFail;
		StencilOp	depthFail;
		StencilOp	depthPass;
	} frontFace, backFace;
};

class DepthStencilState : public Interface
{
public:
};

}
