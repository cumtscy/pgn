#pragma once
namespace pgn {

struct Float3;

class SceneDirectionalLight
{
public:
	virtual void setDirection(Float3* dir) = 0;
};

}
