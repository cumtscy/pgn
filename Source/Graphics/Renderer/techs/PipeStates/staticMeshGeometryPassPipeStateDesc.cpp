#include "Common.h"
#include "PipeStateDesc.h"

using namespace pgn;

static char staticMeshGeometryPassVS[] =
{
	"	uniform EnvCBlock@0 // enum BindingPoint									\n"
	"	{																			\n"
	"		vec3 camPos;															\n"
	"		mat3x4 view;															\n"
	"		mat4 viewProj;															\n"
	"	};																			\n"
	"																				\n"
	"	movable block decl															\n"
	"	material block decl															\n"
	"																				\n"
	"	common vs input decl														\n"
	"	common vs output decl														\n"
	"	flat out lowp float Ks;														\n"
	"	flat out lowp float shininess;												\n"
	"																				\n"
	"	void main()																	\n"
	"	{																			\n"
	"		vec3 pos = position * instances[gl_InstanceID].scale.xyx;				\n"
	"																				\n"
	"		mat3x4 world = instances[gl_InstanceID].world;							\n"
	"																				\n"
	"		world[0][3] -= camPos.x;												\n"
	"		world[1][3] -= camPos.y;												\n"
	"		world[2][3] -= camPos.z;												\n"
	"																				\n"
	"		vec3 wpos = vec4(pos, 1.0) * world;										\n"
	"		vec3 wnor = vec4(normal, 0.0) * world;									\n"
	"																				\n"
	"		gl_Position = vec4(wpos, 1.0) * viewProj;								\n"
	"		n = (vec4(wnor, 0.0) * view + 1.0) * 0.5;								\n"
	"																				\n"
	"		tc0 = texCoord0;														\n"
	"		Ks = materials[materialIndex].Ks;										\n"
	"		shininess = materials[materialIndex].shininess / 100.0;					\n"
	"	}																			\n"
};

static char staticMeshGeometryPassPS[] =
{
	"	common ps input decl														\n"
	"	flat in lowp float Ks;														\n"
	"	flat in lowp float shininess;												\n"
	"																				\n"
	"	common ps output decl														\n"
	"																				\n"
	"	uniform sampler2D diffuseMap#0;												\n"
	"																				\n"
	"	void main()																	\n"
	"	{																			\n"
	"		lowp vec4 diffuse_Ks;													\n"
	"		diffuse_Ks.rgb = texture(diffuseMap, tc0).rgb;							\n"
	"		diffuse_Ks.a = Ks;														\n"
	"																				\n"
	"		lowp vec4 normal_shininess;												\n"
	"		normal_shininess.xyz = n;												\n"
	"		normal_shininess.w = shininess;											\n"
	"																				\n"
	"		color[0] = diffuse_Ks;													\n"
	"		color[1] = normal_shininess;											\n"
	"	}																			\n"
};

static SamplerDesc* samplerDescs[] =
{
	diffuseMapSamplerDesc
};

static PipeStateDesc pipeStateDesc =
{
	staticMeshGeometryPassVS,
	staticMeshGeometryPassPS,
	samplerDescs,
	sizeof(samplerDescs) / sizeof(samplerDescs[0]),
	commonRasterizerStateDesc,
	commonDepthStencilStateDesc,
	blendingDisabledStateDesc
};

PipeStateDesc* staticMeshGeometryPassPipeStateDesc = &pipeStateDesc;
