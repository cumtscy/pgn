#include <PGN/Common/debug_new.h>
#include <PGN/RenderingSystem/Sampler.h>
#include "Program.h"
#include "RenderingSystem.h"
#include "Sampler.h"

struct PDS_DOUTT0
{
	unsigned char reserved0			: 3;
	unsigned char T0_VADDRMODE		: 3;
	unsigned char T0_UADDRMODE		: 3;
	unsigned char T0_MIPFILTER		: 1;
	unsigned char T0_MINFILTER		: 1;
	unsigned char reserved1			: 1;
	unsigned char T0_MAGFILTER		: 1;
	unsigned char reserved2			: 4;
	unsigned char T0_MIPMAPCLAMP	: 4;
	unsigned char T0_DADJUST		: 6;
	unsigned char T0_GAMMA			: 2;
	unsigned char reserved3			: 2;
	unsigned char T0_TEXFEXT		: 1;
};

class Sampler : public pgn::Sampler
{
public:
	union {
		PDS_DOUTT0 controlWord;
		unsigned _controlWord;
	};

	Sampler(pgn::SamplerDesc* desc);
	virtual void dispose();
	virtual void _free();
};

struct MinFilter
{
	unsigned min;
	unsigned mip;
	unsigned mipClamp;
};

const unsigned point = 0;
const unsigned linear = 1;

const MinFilter minFilter[] = { { point, point, SCE_GXM_PDS_DOUTT0_MIPMAPCLAMP_MIN }, { point, point, SCE_GXM_PDS_DOUTT0_MIPMAPCLAMP_MAX }, { point, linear, SCE_GXM_PDS_DOUTT0_MIPMAPCLAMP_MAX }, { linear, point, SCE_GXM_PDS_DOUTT0_MIPMAPCLAMP_MAX }, { linear, linear, SCE_GXM_PDS_DOUTT0_MIPMAPCLAMP_MAX } };
const unsigned magFilter[] = { point, linear };

const unsigned wrap[] = { SCE_GXM_PDS_DOUTT0_UADDRMODE_CLAMP, SCE_GXM_PDS_DOUTT0_UADDRMODE_REPEAT, SCE_GXM_PDS_DOUTT0_UADDRMODE_MIRROR };

Sampler::Sampler(pgn::SamplerDesc* desc)
{
	_controlWord = 0;
	controlWord.T0_MINFILTER = minFilter[desc->minFilter].min;
	controlWord.T0_MIPFILTER = minFilter[desc->minFilter].mip;
	controlWord.T0_MIPMAPCLAMP = minFilter[desc->minFilter].mipClamp;
	controlWord.T0_MAGFILTER = magFilter[desc->magFilter];
	controlWord.T0_UADDRMODE = wrap[desc->wrapU];
	controlWord.T0_VADDRMODE = wrap[desc->wrapV];
}

void Sampler::dispose()
{
}

pgn::Sampler* RenderingSystem::createSampler(pgn::SamplerDesc* desc)
{
	return debug_new Sampler(desc);
}

void Sampler::_free()
{
	delete this;
}

void RenderingSystem::setSamplers(int startSlot, int numSamplers, pgn::Sampler* samplers[])
{
	for (int i = 0; i < numSamplers; i++)
	{
		int textureIndex = startSlot + i;

		unsigned controlWord = textures[textureIndex].controlWords[0] & samplerClearMask;
		textures[textureIndex].controlWords[0] = controlWord | ((Sampler*)samplers[i])->_controlWord;
	}
}
