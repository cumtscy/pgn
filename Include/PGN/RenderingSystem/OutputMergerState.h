#pragma once
#include "../Common/Interface.h"
#include "RenderingSystem.h"
namespace pgn {

enum BlendOp
{
	ADD, SUB, REV_SUB, MIN, MAX
};

enum BlendFactor
{
	ZERO,
	ONE,
	SRC_COLOR,
	INV_SRC_COLOR,
	SRC_ALPHA,
	INV_SRC_ALPHA,
	DEST_COLOR,
	INV_DEST_COLOR,
	DEST_ALPHA,
	INV_DEST_ALPHA,
	reserved_for_BLEND_FACTOR,
	reserved_for_INV_BLEND_FACTOR,
	SRC_ALPHA_SAT
};

const unsigned RED		= 1;
const unsigned GREEN	= 2;
const unsigned BLUE		= 4;
const unsigned ALPHA	= 8;

struct RenderTargetBlendDesc
{
	bool		blendEnable;

	BlendOp		opRGB;
	BlendFactor	factorSrcRGB;
	BlendFactor	factorDestRGB;

	BlendOp		opAlpha;
	BlendFactor	factorSrcAlpha;
	BlendFactor	factorDestAlpha;

	unsigned	writeMask;
};

struct OutputMergerStateDesc
{
	//bool					alphaToCoverageEnable;
	RenderTargetBlendDesc	renderTargets[/*maxNumRenderTargets*/1];
};

class OutputMergerState : public Interface
{
public:
};

}
