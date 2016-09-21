#include <PGN/Graphics/DirectionalLight.h>
#include <PGN/Math/Math.h>
#include "Light.h"

class DirectionalLight : public pgn::DirectionalLight, public Light
{
public:
	pgn::Float4 intensity_spec;

	DirectionalLight(pgn::Pool* pool);
	virtual void dispose();
	virtual void _free();
	virtual void setIntensity(float r, float g, float b, float spec);
};
