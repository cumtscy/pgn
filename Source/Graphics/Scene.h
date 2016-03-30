#include <list>
#include <PGN/Graphics/Scene.h>
#include <vector>
#include "SceneEntity.h"
#include "ScenePointLight.h"

class CBufAllocator;
class Graphics;
struct SceneEntityListItem;

class Scene : public pgn::Scene
{
public:
	Graphics* graphics;

	SceneEntity::StdList sceneEntities;
	ScenePointLight::StdList scenePointLights;

	static const int numLightDisjointSets = 4;
	std::vector<ScenePointLight*> lightDisjointSets[numLightDisjointSets];

	Scene(Graphics* graphics);
	virtual void dispose();
	virtual void _free();

	virtual pgn::SceneEntity* add(pgn::Entity* entity, bool useInstancedDrawing);
	virtual void remove(pgn::SceneEntity* sceneEntity);

	virtual pgn::ScenePointLight* add(pgn::PointLight* light);
	virtual void remove(pgn::ScenePointLight* scenePointLight);

	virtual void commit(pgn::Camera* camera);

	void submitEntities(SceneEntityListItem* first, int count, CBufAllocator* cbufAllocator);
};
