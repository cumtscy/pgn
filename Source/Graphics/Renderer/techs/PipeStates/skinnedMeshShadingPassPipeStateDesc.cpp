#include "Common.h"
#include "PipeStateDesc.h"

using namespace pgn;

extern "C" char skinnedMeshShadingPassVS[];
static char skinnedMeshShadingPassPS[] =
{
	"	forward shading pass env cblock decl										\n"
	"																				\n"
	"	common ps input decl														\n"
	"	in highp vec3 wpos;															\n"
	"	flat in lowp float Ks;														\n"
	"	flat in lowp float shininess;												\n"
	"																				\n"
	"	common ps output decl														\n"
	"																				\n"
	"	uniform sampler2D diffuseMap#0;												\n"
	"	uniform lowp usampler2D lightIndexMap#1;									\n"
	"																				\n"
	"	void main()																	\n"
	"	{																			\n"
	"		lowp vec3 diffuse = texture(diffuseMap, tc0).rgb;						\n"
	"		lowp uvec4 lightIndices = texelFetch(lightIndexMap, ivec2(gl_FragCoord.xy), 0);\n"
	"																				\n"
	"		lowp vec4 I = vec4(0.0, 0.0, 0.0, 1.0);									\n"
	"																				\n"
	"		for (int i = 0; i < 4; i++)												\n"
	"		{																		\n"
	"			lowp uint lightIndex = lightIndices[i];								\n"
	"																				\n"
	"			if (lightIndex == 255u)												\n"
	"				continue;														\n"
	"																				\n"
	"			PointLight light = wPointLights[lightIndex];						\n"
	"																				\n"
	"			highp vec3 lightVec = light.pos_att.xyz - camPos - wpos;			\n"
	"			highp float dist = length(lightVec);								\n"
	"			highp float invDist = 1.0 / dist;									\n"
	"			lowp vec3 L = lightVec * invDist;									\n"
	"			lowp float NdotL = max(0.0, dot(n,L));								\n"
	"			lowp vec3 R = 2.0 * NdotL * n - L;									\n"
	"			lowp vec3 V = normalize(-wpos);										\n"
	"			lowp float VdotR = max(0.0, dot(V,R));								\n"
	"			lowp float att = 1.0 / (light.pos_att.w * dist*dist*dist*dist + 1.0);\n"
	"																				\n"
	"			I.rgb += (diffuse * NdotL + Ks * pow(VdotR, shininess) * light.intensity_spec.a) * att * light.intensity_spec.rgb;\n"
	"		}																		\n"
	"																				\n"
	"		color[0] = I;															\n"
	"	}																			\n"
};

static SamplerDesc* samplerDescs[] =
{
	  diffuseMapSamplerDesc
	, pointSamplerDesc
};

static PipeStateDesc pipeStateDesc =
{
	skinnedMeshShadingPassVS,
	skinnedMeshShadingPassPS,
	samplerDescs,
	sizeof(samplerDescs) / sizeof(samplerDescs[0]),
	commonRasterizerStateDesc,
	commonDepthStencilStateDesc,
	blendingDisabledStateDesc
};

PipeStateDesc* skinnedMeshShadingPassPipeStateDesc = &pipeStateDesc;
