#include "Common.h"
#include "PipeStateDesc.h"

using namespace pgn;

static char lightingPassVS[] =
{
	"	movable 2d block decl														\n"
	"																				\n"
	"	vs 2d input decl															\n"
	"	common vs output decl														\n"
	"																				\n"
	"	void main()																	\n"
	"	{																			\n"
	"		vec2 pos;																\n"
	"		pos = position2D * instances[0].scale;									\n"
	"		pos = vec3(pos, 1.0) * instances[0].transf;								\n"
	"		gl_Position = vec4(pos, -1.0, 1.0);										\n"
	"		tc0 = texCoord0;														\n"
	"	}																			\n"
};

static char lightingPassPS[] =
{
	"	point light struct decl														\n"
	"																				\n"
	"	uniform EnvCBlock@0 // enum BindingPoint									\n"
	"	{																			\n"
	"		highp mat4 invProj;														\n"
	"		PointLight vPointLights[256];											\n"
	"	};																			\n"
	"																				\n"
	"	common ps input decl														\n"
	"	common ps output decl														\n"
	"																				\n"
	"	uniform sampler2D gbuf0#0;													\n"
	"	uniform sampler2D gbuf1#1;													\n"
	"	uniform lowp usampler2D lightIndexMap#2;									\n"
	"	uniform highp sampler2D depthStencilMap#3;									\n"
	"																				\n"
	"	void main()																	\n"
	"	{																			\n"
	"		highp vec2 tc = vec2(tc0.x, 1.0 - tc0.y);								\n"
	"		highp vec2 xy = tc * 2.0 - 1.0;											\n"
	"																				\n"
	"		lowp vec4 diffuse_Ks = texture(gbuf0, tc);								\n"
	"		lowp vec4 normal_shininess = texture(gbuf1, tc);						\n"
	"		lowp uvec4 lightIndices = texture(lightIndexMap, tc);					\n"
	"		highp float depth = texture(depthStencilMap, tc).r * 2.0 - 1.0;			\n"
	"																				\n"
	"		highp vec3 pos;															\n"
	"		pos.z = 1.0 / (depth * invProj[3][2] + invProj[3][3]);					\n"
	"		pos.x = xy.x * invProj[0][0] * pos.z;									\n"
	"		pos.y = xy.y * invProj[1][1] * pos.z;									\n"
	"																				\n"
	"		lowp vec3 N = normal_shininess.xyz * 2.0 - 1.0;							\n"
	"		lowp vec4 I = vec4(0.0, 0.0, 0.0, 1.0);									\n"
	"																				\n"
	"		for (int i = 0; i < 4; i++)												\n"
	"		{																		\n"
	"			lowp uint lightIndex = lightIndices[i];								\n"
	"																				\n"
	"			if (lightIndex == 255u)												\n"
	"				continue;														\n"
	"																				\n"
	"			PointLight light = vPointLights[lightIndex];						\n"
	"																				\n"
	"			highp vec3 lightVec = light.pos_att.xyz - pos;						\n"
	"			highp float dist = length(lightVec);								\n"
	"			highp float invDist = 1.0 / dist;									\n"
	"			lowp vec3 L = lightVec * invDist;									\n"
	"			lowp float NdotL = max(0.0, dot(N,L));								\n"
	"			lowp vec3 R = 2.0 * NdotL * N - L;									\n"
	"			lowp vec3 V = normalize(-pos);										\n"
	"			lowp float VdotR = max(0.0, dot(V,R));								\n"
	"			mediump float shininess = normal_shininess.w * 100.0;				\n"
	"			lowp float att = 1.0 / (light.pos_att.w * dist*dist*dist*dist + 1.0);\n"
	"																				\n"
	"			I.rgb += (diffuse_Ks.rgb * NdotL + diffuse_Ks.a * pow(VdotR, shininess) * light.intensity_spec.a) * att * light.intensity_spec.rgb;\n"
	"		}																		\n"
	"																				\n"
	"		color[0] = I;															\n"
	"	}																			\n"
};

static SamplerDesc* samplerDescs[] =
{
	  pointSamplerDesc
	, pointSamplerDesc
	, pointSamplerDesc
	, pointSamplerDesc
};

static PipeStateDesc pipeStateDesc =
{
	lightingPassVS,
	lightingPassPS,
	samplerDescs,
	sizeof(samplerDescs) / sizeof(samplerDescs[0]),
	commonRasterizerStateDesc,
	depthStencilTestingDisabledStateDesc,
	blendingDisabledStateDesc
};

PipeStateDesc* lightingPassPipeStateDesc = &pipeStateDesc;
