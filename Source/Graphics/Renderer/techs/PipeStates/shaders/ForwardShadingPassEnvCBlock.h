#include "PointLight.h"

uniform EnvCBlock@0 // enum BindingPoint
{
	highp vec3 camPos;
	highp mat4 viewProj;
	PointLight wPointLights[256];
};
