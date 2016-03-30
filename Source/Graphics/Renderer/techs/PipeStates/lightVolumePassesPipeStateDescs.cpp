#include <PGN/RenderingSystem/DepthStencilState.h>
#include <PGN/RenderingSystem/OutputMergerState.h>
#include "Common.h"
#include "PipeStateDesc.h"

using namespace pgn;

static char lightVolumePassesVS[] =
{
	"	uniform EnvCBlock@0 // enum BindingPoint									\n"
	"	{																			\n"
	"		vec3 camPos;															\n"
	"		mat4 viewProj;															\n"
	"	};																			\n"
	"																				\n"
	"	point light volumn instance constant block decl								\n"
	"																				\n"
	"	common vs input decl														\n"
	"																				\n"
	"	void main()																	\n"
	"	{																			\n"
	"		vec3 translation = instances[gl_InstanceID].pos_scale.xyz - camPos;		\n"
	"		vec3 pos = position * instances[gl_InstanceID].pos_scale.w + translation;\n"
	"																				\n"
	"		gl_Position = vec4(pos, 1.0) * viewProj;								\n"
	"	}																			\n"
};

static char lightVolumePassesPS[] =
{
	"	void main()																	\n"
	"	{																			\n"
	"	}																			\n"
};

static DepthStencilStateDesc depthStencilStateDesc =
{
	true,					// depthEnable
	false,					// depthWrite
	LESS,					// depthFunc

	true,					// stencilEnable
	0xff,					// stencilReadMask
	0xff,					// stencilWriteMask

	//frontFace
	ALWAYS,					// stencilFunc
	KEEP,					// stencilFail
	DECR_SAT,				// depthFail
	KEEP,					// depthPass

	//backFace
	ALWAYS,					// stencilFunc
	KEEP,					// stencilFail
	INCR_SAT,				// depthFail
	KEEP					// depthPass
};

static OutputMergerStateDesc outputMergerStateDesc =
{
	//false,					// alphaToCoverageEnable

	//renderTargets[0]
	false,					// blendEnable
	ADD,					// opRGB
	INV_DEST_ALPHA,			// factorSrcRGB
	DEST_ALPHA,				// factorDestRGB
	ADD,					// opAlpha
	ZERO,					// factorSrcAlpha
	ONE,					// factorDestAlpha

	0						// writeMask
};

static PipeStateDesc pass1 =
{
	lightVolumePassesVS,
	lightVolumePassesPS,
	0,
	0,
	backFaceRasterizerStateDesc,
	&depthStencilStateDesc,
	&outputMergerStateDesc
};

static PipeStateDesc pass2 =
{
	lightVolumePassesVS,
	lightVolumePassesPS,
	0,
	0,
	commonRasterizerStateDesc,
	&depthStencilStateDesc,
	&outputMergerStateDesc
};

PipeStateDesc* lightVolumeBackFacePassPipeStateDesc = &pass1;
PipeStateDesc* lightVolumeFrontFacePassPipeStateDesc = &pass2;
