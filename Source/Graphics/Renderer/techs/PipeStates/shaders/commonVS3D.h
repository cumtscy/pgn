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
#include "commonVS3DInput.h"

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
