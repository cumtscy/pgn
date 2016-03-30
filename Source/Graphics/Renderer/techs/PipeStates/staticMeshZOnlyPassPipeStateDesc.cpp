#include "Common.h"
#include "PipeStateDesc.h"

using namespace pgn;

static char staticMeshZOnlyPassVS[] =
{
	"	uniform EnvCBlock@0 // enum BindingPoint									\n"
	"	{																			\n"
	"		vec3 camPos;															\n"
	"		mat4 viewProj;															\n"
	"	};																			\n"
	"																				\n"
	"	movable block decl															\n"
	"																				\n"
	"	in vec3 position;															\n"
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
	"																				\n"
	"		gl_Position = vec4(wpos, 1.0) * viewProj;								\n"
	"	}																			\n"
};

static char staticMeshZOnlyPassPS[] =
{
	"	void main()																	\n"
	"	{																			\n"
	"	}																			\n"
};

static PipeStateDesc pipeStateDesc =
{
	staticMeshZOnlyPassVS,
	staticMeshZOnlyPassPS,
	0,
	0,
	commonRasterizerStateDesc,
	commonDepthStencilStateDesc,
	blendingDisabledStateDesc
};

PipeStateDesc* staticMeshZOnlyPassPipeStateDesc = &pipeStateDesc;
