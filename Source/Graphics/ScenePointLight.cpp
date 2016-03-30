#include "ScenePointLight.h"

ScenePointLight::ScenePointLight(PointLight* pointLight)
{
	this->pointLight = pointLight;
}

void ScenePointLight::setPosition(pgn::Float3* pos)
{
	this->pos = *pos;
}
