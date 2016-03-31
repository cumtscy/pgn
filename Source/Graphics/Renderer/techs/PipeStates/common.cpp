#include <PGN/RenderingSystem/DepthStencilState.h>
#include <PGN/RenderingSystem/OutputMergerState.h>
#include <PGN/RenderingSystem/RasterizerState.h>
#include <PGN/RenderingSystem/Sampler.h>

using namespace pgn;

static char movableBlockDeclMacro[] =
{
	"movable block decl"
};
static char movableBlockDecl[] =
{
	"																\n"
	"	struct Movable												\n"
	"	{															\n"
	"		mat3x4 world;											\n"
	"		vec2 scale;												\n"
	"	};															\n"
	"																\n"
	"	uniform MovableBlock@1				// enum BindingPoint	\n"
	"	{															\n"
	"		Movable instances[256];									\n"
	"	};															"
};


static char movable2DBlockDeclMacro[] =
{
	"movable 2d block decl"
};
static char movable2DBlockDecl[] =
{
	"																\n"
	"	struct Movable2D											\n"
	"	{															\n"
	"		mat2x3 transf;											\n"
	"		vec2 scale;												\n"
	"	};															\n"
	"																\n"
	"	uniform Movable2DBlock@1			// enum BindingPoint	\n"
	"	{															\n"
	"		Movable2D instances[256];								\n"
	"	};															"
};


static char pointLightVolumnInstanceCBlockDeclMacro[] =
{
	"point light volumn instance constant block decl"
};
static char pointLightVolumnInstanceCBlockDecl[] =
{
	"																\n"
	"	struct PointLightVolumnInstance								\n"
	"	{															\n"
	"		highp vec4 pos_scale;									\n"
	"		highp int lightIndex;									\n"
	"	};															\n"
	"																\n"
	"	uniform PointLightVolumnInstanceCBlock@1	// enum BindingPoint	\n"
	"	{															\n"
	"		PointLightVolumnInstance instances[256];				\n"
	"	};															"
};


static char boneMatBlockDeclMacro[] =
{
	"bone mat block decl"
};
static char boneMatBlockDecl[] =
{
	"																\n"
	"	uniform BoneMatBlock@2				// enum BindingPoint	\n"
	"	{															\n"
	"		mat3x4 model[128];										\n"
	"	};															"
};


static char materialBlockDeclMacro[] =
{
	"material block decl"
};
static char materialBlockDecl[] =
{
	"																\n"
	"	struct Material												\n"
	"	{															\n"
	"		highp float Ks;											\n"
	"		highp float shininess;									\n"
	"	};															\n"
	"																\n"
	"	uniform MaterialBlock@3				// enum BindingPoint	\n"
	"	{															\n"
	"		Material materials[256];								\n"
	"	};															"
};


static char forwardShadingPassEnvCBlockDeclMacro[] =
{
	"forward shading pass env cblock decl"
};
static char forwardShadingPassEnvCBlockDecl[] =
{
	"	point light struct decl										\n"
	"																\n"
	"	uniform EnvCBlock@0 // enum BindingPoint					\n"
	"	{															\n"
	"		highp vec3 camPos;										\n"
	"		highp mat4 viewProj;									\n"
	"		PointLight wPointLights[256];							\n"
	"	};															\n"
};


static char pointLightStructDeclMacro[] =
{
	"point light struct decl"
};
static char pointLightStructDecl[] =
{
	"																\n"
	"	struct PointLight											\n"
	"	{															\n"
	"		highp vec4 intensity_spec;								\n"
	"		highp vec4 pos_att;										\n"
	"	};															\n"
};


static char commonVSInputDeclMacro[] =
{
	"common vs input decl"
};
static char commonVSInputDecl[] =
{
	"								\n"
	"	in vec3 position;			\n"
	"	in vec3 normal;				\n"
	"	in int numAssignedBones;	\n"
	"	in uvec4 boneIndices;		\n"
	"	in vec4 boneWeights;		\n"
	"	in uint materialIndex;		\n"
	"	in vec2 texCoord0;			\n"
	"	in vec2 texCoord1;			"
};


static char commonVS2DInputDeclMacro[] =
{
	"vs 2d input decl"
};
static char commonVS2DInputDecl[] =
{
	"								\n"
	"	in vec2 position2D;			\n"
	"	in vec2 texCoord0;			"
};


static char commonVSOutputDeclMacro[] =
{
	"common vs output decl"
};
static char commonVSOutputDecl[] =
{
	"								\n"
	"	out lowp vec3 n;			\n"
	"	out vec2 tc0;				\n"
	"	out vec2 tc1;				"
};


static char commonPSInputDeclMacro[] =
{
	"common ps input decl"
};
static char commonPSInputDecl[] =
{
	"								\n"
	"	in lowp vec3 n;				\n"
	"	in highp vec2 tc0;			\n"
	"	in highp vec2 tc1;			"
};


static char commonPSOutputDeclMacro[] =
{
	"common ps output decl"
};
static char commonPSOutputDecl[] =
{
	"								\n"
	"	out lowp vec4 color[4];		"
};

char* shaderMacros[] =
{
	  movableBlockDeclMacro
	, movable2DBlockDeclMacro
	, pointLightVolumnInstanceCBlockDeclMacro
	, boneMatBlockDeclMacro
	, materialBlockDeclMacro
	, forwardShadingPassEnvCBlockDeclMacro
	, pointLightStructDeclMacro
	, commonVSInputDeclMacro
	, commonVS2DInputDeclMacro
	, commonVSOutputDeclMacro
	, commonPSInputDeclMacro
	, commonPSOutputDeclMacro
};

char* shaderMacroDefs[] =
{
	  movableBlockDecl
	, movable2DBlockDecl
	, pointLightVolumnInstanceCBlockDecl
	, boneMatBlockDecl
	, materialBlockDecl
	, forwardShadingPassEnvCBlockDecl
	, pointLightStructDecl
	, commonVSInputDecl
	, commonVS2DInputDecl
	, commonVSOutputDecl
	, commonPSInputDecl
	, commonPSOutputDecl
};

int numShaderMacros = sizeof(shaderMacros) / sizeof(shaderMacros[0]);


static SamplerDesc _diffuseMapSamplerDesc =
{
	MIN_LINEAR_MIP_LINEAR,	// MinFilter
	MAG_LINEAR,				// MagFilter
	CLAMP,					// wrapU
	CLAMP,					// wrapV
	16,						// maxAniso
	ALWAYS					// cmpFunc
};
SamplerDesc* diffuseMapSamplerDesc = &_diffuseMapSamplerDesc;


static SamplerDesc _pointSamplerDesc =
{
	MIN_POINT,				// MinFilter
	MAG_POINT,				// MagFilter
	CLAMP,					// wrapU
	CLAMP,					// wrapV
	1,						// maxAniso
	ALWAYS					// cmpFunc
};
SamplerDesc* pointSamplerDesc = &_pointSamplerDesc;


static RasterizerStateDesc _commonRasterizerStateDesc =
{
	CCW,					// FrontFace
	BACK_FACE				// CullFace
};
RasterizerStateDesc* commonRasterizerStateDesc = &_commonRasterizerStateDesc;


static RasterizerStateDesc _backFaceRasterizerStateDesc =
{
	CCW,					// FrontFace
	FRONT_FACE				// CullFace
};
RasterizerStateDesc* backFaceRasterizerStateDesc = &_backFaceRasterizerStateDesc;


static DepthStencilStateDesc _commonDepthStencilStateDesc =
{
	true,					// depthEnable
	true,					// depthWrite
	LESS,					// depthFunc

	false,					// stencilEnable
	0xff,					// stencilReadMask
	0xff,					// stencilWriteMask

	//frontFace
	ALWAYS,					// stencilFunc
	KEEP,					// stencilFail
	KEEP,					// depthFail
	KEEP,					// depthPass

	//backFace
	ALWAYS,					// stencilFunc
	KEEP,					// stencilFail
	KEEP,					// depthFail
	KEEP					// depthPass
};
DepthStencilStateDesc* commonDepthStencilStateDesc = &_commonDepthStencilStateDesc;


static DepthStencilStateDesc _depthStencilTestingDisabled =
{
	false,					// depthEnable
	false,					// depthWrite
	ALWAYS,					// depthFunc

	false,					// stencilEnable
	0xff,					// stencilReadMask
	0xff,					// stencilWriteMask

	//frontFace
	ALWAYS,					// stencilFunc
	KEEP,					// stencilFail
	KEEP,					// depthFail
	KEEP,					// depthPass

	//backFace
	ALWAYS,					// stencilFunc
	KEEP,					// stencilFail
	KEEP,					// depthFail
	KEEP					// depthPass
};
DepthStencilStateDesc* depthStencilTestingDisabledStateDesc = &_depthStencilTestingDisabled;


static OutputMergerStateDesc _blendingDisabled =
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

	RED|GREEN|BLUE|ALPHA	// writeMask
};
OutputMergerStateDesc* blendingDisabledStateDesc = &_blendingDisabled;


static OutputMergerStateDesc _alphaBlending =
{
	//false,					// alphaToCoverageEnable

	//renderTargets[0]
	true,					// blendEnable
	ADD,					// opRGB
	INV_DEST_ALPHA,			// factorSrcRGB
	DEST_ALPHA,				// factorDestRGB
	ADD,					// opAlpha
	ZERO,					// factorSrcAlpha
	ONE,					// factorDestAlpha

	RED|GREEN|BLUE|ALPHA	// writeMask
};
OutputMergerStateDesc* alphaBlendingStateDesc = &_alphaBlending;
