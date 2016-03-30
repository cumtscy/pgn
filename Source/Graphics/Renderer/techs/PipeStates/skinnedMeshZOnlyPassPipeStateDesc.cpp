#include "Common.h"
#include "PipeStateDesc.h"

using namespace pgn;

static char skinnedMeshZOnlyPassVS[] =
{
	"	uniform EnvCBlock@0 // enum BindingPoint									\n"
	"	{																			\n"
	"		vec3 camPos;															\n"
	"		mat4 viewProj;															\n"
	"	};																			\n"
	"																				\n"
	"	movable block decl															\n"
	"	bone mat block decl															\n"
	"																				\n"
	"	in vec3 position;															\n"
	"	in int numAssignedBones;													\n"
	"	in uvec4 boneIndices;														\n"
	"	in vec4 boneWeights;														\n"
	"																				\n"
	"	void main()																	\n"
	"	{																			\n"
	"		vec3 mpos = vec3(0.0, 0.0, 0.0);										\n"
	"																				\n"
	"		for(int i = 0; i < numAssignedBones; i++)								\n"
	"		{																		\n"
	"			mpos += vec4(position, 1.0) * model[boneIndices[i]] * boneWeights[i];\n"
	"		}																		\n"
	"																				\n"
	"		mpos *= instances[gl_InstanceID].scale.xyx;								\n"
	"																				\n"
	"		mat3x4 world = instances[gl_InstanceID].world;							\n"
	"																				\n"
	"		world[0][3] -= camPos.x;												\n"
	"		world[1][3] -= camPos.y;												\n"
	"		world[2][3] -= camPos.z;												\n"
	"																				\n"
	"		vec3 wpos = vec4(mpos, 1.0) * world;									\n"
	"																				\n"
	"		gl_Position = vec4(wpos, 1.0) * viewProj;								\n"
	"	}																			\n"
};

static char skinnedMeshZOnlyPassPS[] =
{
	"	void main()																	\n"
	"	{																			\n"
	"	}																			\n"
};

static PipeStateDesc pipeStateDesc =
{
	skinnedMeshZOnlyPassVS,
	skinnedMeshZOnlyPassPS,
	0,
	0,
	commonRasterizerStateDesc,
	commonDepthStencilStateDesc,
	blendingDisabledStateDesc
};

PipeStateDesc* skinnedMeshZOnlyPassPipeStateDesc = &pipeStateDesc;
