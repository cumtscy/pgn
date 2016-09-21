#pragma once
#include "../Common/Interface.h"
namespace pgn {

class Camera;
class DirectionalLight;
class Entity;
class PointLight;
class SceneDirectionalLight;
class SceneEntity;
class ScenePointLight;

class Scene : public Interface
{
public:
	virtual SceneEntity* add(Entity* entity, bool useInstancedDrawing) = 0;
	virtual void remove(SceneEntity* sceneEntity) = 0;

	virtual ScenePointLight* add(PointLight* light) = 0;
	virtual void remove(ScenePointLight* scenePointLight) = 0;

	virtual SceneDirectionalLight* add(DirectionalLight* light) = 0;
	virtual void remove(SceneDirectionalLight* sceneDirLight) = 0;

	virtual void commit(Camera* camera) = 0;
};

}
