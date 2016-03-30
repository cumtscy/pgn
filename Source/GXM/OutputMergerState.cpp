#include <PGN/Common/debug_new.h>
#include <PGN/RenderingSystem/OutputMergerState.h>
#include "RenderingSystem.h"

class OutputMergerState : public pgn::OutputMergerState
{
public:
	SceGxmBlendInfo blendInfo;

	OutputMergerState(pgn::OutputMergerStateDesc* desc);
	virtual void dispose();
	virtual void _free();
	virtual void destroy();
};

OutputMergerState::OutputMergerState(pgn::OutputMergerStateDesc* desc)
{
	pgn::RenderTargetBlendDesc* blendDesc = &desc->renderTargets[0];

	unsigned char mask = blendDesc->writeMask;
	unsigned char maskA = mask & pgn::ALPHA >> 3;
	blendInfo.colorMask = mask << 1 | maskA;

	static SceGxmBlendFunc func[] =
	{
		SCE_GXM_BLEND_FUNC_ADD, SCE_GXM_BLEND_FUNC_SUBTRACT, SCE_GXM_BLEND_FUNC_REVERSE_SUBTRACT, SCE_GXM_BLEND_FUNC_MIN, SCE_GXM_BLEND_FUNC_MAX
	};

	static SceGxmBlendFactor factor[] =
	{
		SCE_GXM_BLEND_FACTOR_ZERO,
		SCE_GXM_BLEND_FACTOR_ONE,
		SCE_GXM_BLEND_FACTOR_SRC_COLOR,
		SCE_GXM_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
		SCE_GXM_BLEND_FACTOR_SRC_ALPHA,
		SCE_GXM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
		SCE_GXM_BLEND_FACTOR_DST_COLOR,
		SCE_GXM_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
		SCE_GXM_BLEND_FACTOR_DST_ALPHA,
		SCE_GXM_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
		SCE_GXM_BLEND_FACTOR_ZERO,
		SCE_GXM_BLEND_FACTOR_ZERO,
		SCE_GXM_BLEND_FACTOR_SRC_ALPHA_SATURATE
	};

	if (blendDesc->blendEnable)
	{
		blendInfo.colorFunc = func[blendDesc->opRGB];
		blendInfo.alphaFunc = func[blendDesc->opAlpha];
		blendInfo.colorSrc = factor[blendDesc->factorSrcRGB];
		blendInfo.colorDst = factor[blendDesc->factorDestRGB];
		blendInfo.alphaSrc = factor[blendDesc->factorSrcAlpha];
		blendInfo.alphaDst = factor[blendDesc->factorDestAlpha];
	}
	else
	{
		blendInfo.colorFunc = SCE_GXM_BLEND_FUNC_NONE;
		blendInfo.alphaFunc = SCE_GXM_BLEND_FUNC_NONE;
	}
}

void OutputMergerState::dispose()
{
}

pgn::OutputMergerState* RenderingSystem::createOutputMergerState(pgn::OutputMergerStateDesc* desc)
{
	return debug_new OutputMergerState(desc);
}

void OutputMergerState::_free()
{
	delete this;
}

void OutputMergerState::destroy()
{
	delete this;
}

void RenderingSystem::setOutputMergerState(pgn::OutputMergerState* state)
{
	blendInfo = ((OutputMergerState*)state)->blendInfo;
}
