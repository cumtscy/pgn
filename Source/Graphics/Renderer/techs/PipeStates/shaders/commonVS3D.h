#include "commonVSOutput.h"

struct Movable
{
	mat3x4 world;
	vec2 scale;
};

uniform MovableBlock@1				// enum BindingPoint
{
	Movable instances[256];
};

uniform BoneMatBlock@2				// enum BindingPoint
{
	mat3x4 model[128];
};

struct Material
{
	highp float Ks;
	highp float shininess;
};

uniform MaterialBlock@3				// enum BindingPoint
{
	Material materials[256];
};

in vec3 position;
in vec3 normal;
in int numAssignedBones;
in uvec4 boneIndices;
in vec4 boneWeights;
in uint materialIndex;
in vec2 texCoord0;
in vec2 texCoord1;
