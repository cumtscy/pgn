#include "DirectionalLight.h"
#include "Graphics.h"

DirectionalLight::DirectionalLight(pgn::Pool* pool)
	: Light(pool)
{
}

void DirectionalLight::dispose()
{
}

pgn::DirectionalLight* Graphics::createDirectionalLight()
{
	return new(dirLightPool->alloc()) DirectionalLight(dirLightPool);
}

void DirectionalLight::_free()
{
	pool->_free(this);
}

void DirectionalLight::setIntensity(float r, float g, float b, float spec)
{
	intensity_spec[0] = r;
	intensity_spec[1] = g;
	intensity_spec[2] = b;
	intensity_spec[3] = spec;
}
