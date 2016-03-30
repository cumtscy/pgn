#pragma once
namespace pgn {

struct Float4x3;

class SceneEntity
{
public:
	virtual void setScale(float xz, float y) = 0;
	virtual void setWorldMat(Float4x3* mat) = 0;
};

}
