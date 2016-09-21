#pragma once
#include <list>
#include <PGN/Graphics/SceneDirectionalLight.h>
#include <PGN/Math/Math.h>
#include <PGN/Utilities/Pool.h>
#include <PGN/Utilities/PoolAllocator.h>

class DirectionalLight;

class SceneDirectionalLight : public pgn::SceneDirectionalLight
{
public:
	typedef pgn::PoolAllocator<SceneDirectionalLight, pgn::Pool> StdListAllocator;
	typedef std::list<SceneDirectionalLight, StdListAllocator> StdList;

	StdList::iterator it;
	DirectionalLight* dirLight;
	pgn::Float3 dir;

	SceneDirectionalLight(DirectionalLight* dirLight);
	virtual void setDirection(pgn::Float3* dir);
};
