#include <PGN/Graphics/PointLight.h>
#include <PGN/Math/Math.h>
#include "Light.h"

class PointLight : public pgn::PointLight, public Light
{
public:
	pgn::Float4 intensity_spec;
	float radius;
	float att;

	PointLight(pgn::Pool* pool);
	virtual void dispose();
	virtual void _free();
	virtual void setIntensity(float r, float g, float b, float spec);
	virtual void setRadius(float r);
};
