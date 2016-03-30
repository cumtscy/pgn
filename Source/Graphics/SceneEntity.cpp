#include "SceneEntity.h"

SceneEntity::SceneEntity(Entity* entity, bool useInstancedDrawing)
{
	this->entity = entity;
	this->useInstancedDrawing = useInstancedDrawing;
}

void SceneEntity::setScale(float xz, float y)
{
	movable.scaleXZ = xz;
	movable.scaleY = y;
}

void SceneEntity::setWorldMat(pgn::Float4x3* mat)
{
	movable.worldMat = *mat;
}
