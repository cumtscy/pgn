#include "SceneDirectionalLight.h"

SceneDirectionalLight::SceneDirectionalLight(DirectionalLight* dirLight)
{
	this->dirLight = dirLight;
}

void SceneDirectionalLight::setDirection(pgn::Float3* dir)
{
	this->dir = *dir;
}
