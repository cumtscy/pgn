#include "Graphics.h"
#include "PointLight.h"

PointLight::PointLight(pgn::Pool* pool)
	: Light(pool)
{
}

void PointLight::dispose()
{
}

pgn::PointLight* Graphics::createPointLight()
{
	return new(pointLightPool->alloc()) PointLight(pointLightPool);
}

void PointLight::_free()
{
	pool->_free(this);
}

void PointLight::setIntensity(float r, float g, float b, float spec)
{
	intensity_spec[0] = r;
	intensity_spec[1] = g;
	intensity_spec[2] = b;
	intensity_spec[3] = spec;
}

void PointLight::setRadius(float r)
{
	radius = r;

	float epsilon = 0.05f;
	att = (1.0f / epsilon - 1.0f) / (r*r*r*r);
}
