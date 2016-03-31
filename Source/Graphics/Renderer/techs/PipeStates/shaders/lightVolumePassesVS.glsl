#include "commonVS3DInput.h"
#include "PointLightVolumnInstanceCBlock.h"

uniform EnvCBlock@0 // enum BindingPoint
{
	vec3 camPos;
	mat4 viewProj;
};

void main()
{
	vec3 translation = instances[gl_InstanceID].pos_scale.xyz - camPos;
	vec3 pos = position * instances[gl_InstanceID].pos_scale.w + translation;

	gl_Position = vec4(pos, 1.0) * viewProj;
}
