#pragma once
namespace pgn {

struct Float3;

class ScenePointLight
{
public:
	virtual void setPosition(Float3* pos) = 0;
};

}
