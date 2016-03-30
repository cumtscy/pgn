#include <PGN/RenderingSystem/DepthStencilState.h>
#include <PGN/RenderingSystem/OutputMergerState.h>
#include "Common.h"
#include "PipeStateDesc.h"

using namespace pgn;

static char lightIndexingPassesVS[] =
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
	"	flat out uint lightIndex;													\n"
	"																				\n"
	"	void main()																	\n"
	"	{																			\n"
	"		lightIndex = uint(instances[gl_InstanceID].lightIndex);					\n"
	"																				\n"
	"		vec3 translation = instances[gl_InstanceID].pos_scale.xyz - camPos;		\n"
	"		vec3 pos = position * instances[gl_InstanceID].pos_scale.w + translation;\n"
	"																				\n"
	"		gl_Position = vec4(pos, 1.0) * viewProj;								\n"
	"	}																			\n"
};

static char lightIndexingPassesPS[] =
{
	"	flat in uint lightIndex;													\n"
	"	out uvec4 color[4];															\n"
	"																				\n"
	"	void main()																	\n"
	"	{																			\n"
	"		color[0] = uvec4(lightIndex);											\n"
	"	}																			\n"
};

static DepthStencilStateDesc depthStencilStateDesc =
{
	false,					// depthEnable
	false,					// depthWrite
	ALWAYS,					// depthFunc

	true,					// stencilEnable
	0xff,					// stencilReadMask
	0xff,					// stencilWriteMask

	//frontFace
	ALWAYS,					// stencilFunc
	KEEP,					// stencilFail
	KEEP,					// depthFail
	KEEP,					// depthPass

	//backFace
	NOT_EQUAL,				// stencilFunc
	KEEP,					// stencilFail
	KEEP,					// depthFail
	KEEP					// depthPass
};

static OutputMergerStateDesc outputMergerStatePass1 =
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

	RED						// writeMask
};

static OutputMergerStateDesc outputMergerStatePass2 =
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

	GREEN					// writeMask
};

static OutputMergerStateDesc outputMergerStatePass3 =
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

	BLUE					// writeMask
};

static OutputMergerStateDesc outputMergerStatePass4 =
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

	ALPHA					// writeMask
};

static PipeStateDesc pass1 =
{
	lightIndexingPassesVS,
	lightIndexingPassesPS,
	0,
	0,
	backFaceRasterizerStateDesc,
	&depthStencilStateDesc,
	&outputMergerStatePass1
};

static PipeStateDesc pass2 =
{
	lightIndexingPassesVS,
	lightIndexingPassesPS,
	0,
	0,
	backFaceRasterizerStateDesc,
	&depthStencilStateDesc,
	&outputMergerStatePass2
};

static PipeStateDesc pass3 =
{
	lightIndexingPassesVS,
	lightIndexingPassesPS,
	0,
	0,
	backFaceRasterizerStateDesc,
	&depthStencilStateDesc,
	&outputMergerStatePass3
};

static PipeStateDesc pass4 =
{
	lightIndexingPassesVS,
	lightIndexingPassesPS,
	0,
	0,
	backFaceRasterizerStateDesc,
	&depthStencilStateDesc,
	&outputMergerStatePass4
};

PipeStateDesc* lightIndexingPass1PipeStateDesc = &pass1;
PipeStateDesc* lightIndexingPass2PipeStateDesc = &pass2;
PipeStateDesc* lightIndexingPass3PipeStateDesc = &pass3;
PipeStateDesc* lightIndexingPass4PipeStateDesc = &pass4;
