#pragma once
#include <list>
#include <PGN/Graphics/ScenePointLight.h>
#include <PGN/Math/Math.h>
#include <PGN/Utilities/Pool.h>
#include <PGN/Utilities/PoolAllocator.h>

class PointLight;

class ScenePointLight : public pgn::ScenePointLight
{
public:
	typedef pgn::PoolAllocator<ScenePointLight, pgn::Pool> StdListAllocator;
	typedef std::list<ScenePointLight, StdListAllocator> StdList;

	StdList::iterator it;
	PointLight* pointLight;
	pgn::Float3 pos;

	ScenePointLight(PointLight* pointLight);
	virtual void setPosition(pgn::Float3* pos);
};
