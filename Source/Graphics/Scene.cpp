#include <PGN/Common/debug_new.h>
#include <PGN/SkeletalAnimation/Skeleton.h>
#include <PGN/SkeletalAnimation/SkeletonTemplate.h>
#include <PGN/Utilities/HeapAllocator.h>
#include <PGN/Utilities/ResourceManager/ResourceHandle.h>
#include <PGN/Utilities/ResourceManager/ResourceManager.h>
#include "Camera.h"
#include "Entity.h"
#include "Graphics.h"
#include "Model.h"
#include "PointLight.h"
#include "Renderer/CBufAllocator.h"
#include "Renderer/Movable2D.h"
#include "Scene.h"

Scene::Scene(Graphics* graphics)
{
	this->graphics = graphics;

	for (auto& set : lightDisjointSets)
		set.reserve(FrameContext::maxNumPointLights);
}

void Scene::dispose()
{
}

pgn::Scene* Graphics::createScene()
{
	return debug_new Scene(this);
}

void Scene::_free()
{
	delete this;
}

pgn::SceneEntity* Scene::add(pgn::Entity* entity, bool useInstancedDrawing)
{
	sceneEntities.emplace_front((Entity*)entity, useInstancedDrawing);
	SceneEntity* sceneEntity = &sceneEntities.front();
	sceneEntity->it = sceneEntities.begin();
	return sceneEntity;
}

void Scene::remove(pgn::SceneEntity* _sceneEntity)
{
	SceneEntity* sceneEntity = (SceneEntity*)_sceneEntity;
	sceneEntities.erase(sceneEntity->it);
}

pgn::ScenePointLight* Scene::add(pgn::PointLight* light)
{
	scenePointLights.emplace_front((PointLight*)light);
	ScenePointLight* scenePointLight = &scenePointLights.front();
	scenePointLight->it = scenePointLights.begin();
	return scenePointLight;
}

void Scene::remove(pgn::ScenePointLight* _scenePointLight)
{
	ScenePointLight* scenePointLight = (ScenePointLight*)_scenePointLight;
	scenePointLights.erase(scenePointLight->it);
}

struct SceneEntityListItem
{
	SceneEntity* sceneEntity;
	SceneEntityListItem* next;
};

class SceneEntityList
{
public:
	SceneEntityListItem* first;
	int size;

	SceneEntityList()
	{
		size = 0;
	}
};

const int maxInstanceCount = 256;

#ifndef min
#define min(a,b) (a < b ? a : b)
#endif

void Scene::commit(pgn::Camera* _camera)
{
	FrameContext* frameContext = graphics->renderer.beginSubmit();

	if (!frameContext)
		return;

	graphics->performPendingRemovals();

	CBufAllocator* cbufAllocator = frameContext->cbufAllocator;
	pgn::Heap* tmpBuf = graphics->tmpBuf;

	typedef pgn::HeapAllocator<std::pair<Entity*, SceneEntityList>> SceneEntityGroupAllocator;
	typedef std::map<Entity*, SceneEntityList, std::less<Entity*>, SceneEntityGroupAllocator> SceneEntityGroupMap;
	SceneEntityGroupMap* _sceneEntityGroupMap = new(tmpBuf->alloc(sizeof(SceneEntityGroupMap))) SceneEntityGroupMap(tmpBuf);
	SceneEntityGroupMap& sceneEntityGroupMap = *_sceneEntityGroupMap;

	for (auto& _sceneEntity : sceneEntities)
	{
		SceneEntity* sceneEntity = (SceneEntity*)&_sceneEntity;
		if (!sceneEntity->useInstancedDrawing)
		{
			SceneEntityListItem item;
			item.sceneEntity = sceneEntity;
			submitEntities(&item, 1, cbufAllocator);
		}
		else
		{
			SceneEntityList* sceneEntityList = &sceneEntityGroupMap[sceneEntity->entity];
			SceneEntityListItem* item = (SceneEntityListItem*)tmpBuf->alloc(sizeof(SceneEntityListItem));
			item->sceneEntity = sceneEntity;
			item->next = sceneEntityList->first;
			sceneEntityList->first = item;
			sceneEntityList->size++;
		}
	}

	for (auto& entry : sceneEntityGroupMap)
	{
		SceneEntityList sceneEntityList = entry.second;
		submitEntities(sceneEntityList.first, sceneEntityList.size, cbufAllocator);
	}

	tmpBuf->clear();

	ScenePointLight* scenePointLightArray[FrameContext::maxNumPointLights];
	_aligned(16, float pos[3][FrameContext::maxNumPointLights]);
	_aligned(16, float r[FrameContext::maxNumPointLights]);
	_aligned(16, float x[FrameContext::maxNumPointLights * 2]);
	_aligned(16, float y[FrameContext::maxNumPointLights * 2]);
	_aligned(16, float z[FrameContext::maxNumPointLights * 2]);

	float* minX = x;
	float* maxX = x + FrameContext::maxNumPointLights;
	float* minY = y;
	float* maxY = y + FrameContext::maxNumPointLights;
	float* minZ = z;
	float* maxZ = z + FrameContext::maxNumPointLights;

	int i = 0;
	for (auto& scenePointLight : scenePointLights)
	{
		if (i >= FrameContext::maxNumPointLights)
			break;

		scenePointLightArray[i] = &scenePointLight;
		pos[0][i] = scenePointLight.pos[0];
		pos[1][i] = scenePointLight.pos[1];
		pos[2][i] = scenePointLight.pos[2];
		r[i] = scenePointLight.pointLight->radius;
		i++;
	}

	int numScenePointLights = i;

	int n = numScenePointLights / 4;
	if (numScenePointLights % 4) n++;

	pgn::sub((pgn::Float4*)minX, (pgn::Float4*)pos[0], (pgn::Float4*)r, n);
	pgn::add((pgn::Float4*)maxX, (pgn::Float4*)pos[0], (pgn::Float4*)r, n);
	pgn::sub((pgn::Float4*)minY, (pgn::Float4*)pos[1], (pgn::Float4*)r, n);
	pgn::add((pgn::Float4*)maxY, (pgn::Float4*)pos[1], (pgn::Float4*)r, n);
	pgn::sub((pgn::Float4*)minZ, (pgn::Float4*)pos[2], (pgn::Float4*)r, n);
	pgn::add((pgn::Float4*)maxZ, (pgn::Float4*)pos[2], (pgn::Float4*)r, n);

	for (auto& set : lightDisjointSets)
		set.clear();

	unsigned char disjointSetMasks[FrameContext::maxNumPointLights];
	memset(disjointSetMasks, 0, sizeof(disjointSetMasks));

	for (int a = 0; a < numScenePointLights; a++)
	{
		unsigned char mask = 0;

		for (int i = 0; i < 4; i++)
		{
			if ((disjointSetMasks[a] & (1 << i)) == 0)
			{
				lightDisjointSets[i].push_back(scenePointLightArray[a]);
				mask = 1 << i;
				break;
			}
		}

		//if (!mask) continue;

		for (int b = a + 1; b < numScenePointLights; b++)
		{
			if (minX[a] > maxX[b] || maxX[a] < minX[b])	continue;
			if (minY[a] > maxY[b] || maxY[a] < minY[b])	continue;
			if (minZ[a] > maxZ[b] || maxZ[a] < minZ[b])	continue;

			float dx = pos[0][a] - pos[0][b];
			float dy = pos[1][a] - pos[1][b];
			float dz = pos[2][a] - pos[2][b];
			float R = r[a] + r[b];

			if (dx*dx + dy*dy + dz*dz > R*R)
				continue;

			disjointSetMasks[b] |= mask;
		}
	}

	Batch batch;

	batch.geom = (Geometry*)graphics->renderer.geomMgr->peekResource("sphere");
	batch.textureInfo = 0;

	i = 0;

	for (int j = 0; j < numLightDisjointSets; j++)
	{
		auto& set = lightDisjointSets[j];
		auto it = set.begin();

		for (int count = (int)set.size(); count > 0; count -= batch.instanceCount)
		{
			struct PointLightVolumnInstance
			{
				pgn::Float4 pos_scale;
				int lightIndex;
			};

			batch.instanceCount = min(count, maxInstanceCount);
			PointLightVolumnInstance* instances = (PointLightVolumnInstance*)cbufAllocator->alloc(sizeof(PointLightVolumnInstance) * batch.instanceCount, &batch.instanceCBlockBuf);

			for (int k = 0; k < batch.instanceCount; k++)
			{
				ScenePointLight* scenePointLight = *it++;
				PointLight* light = scenePointLight->pointLight;

				instances[k].pos_scale = scenePointLight->pos;
				instances[k].pos_scale[3] = light->radius;
				instances[k].lightIndex = i;

				frameContext->wPointLights[i].intensity_spec = light->intensity_spec;
				frameContext->wPointLights[i].pos_att = scenePointLight->pos;
				frameContext->wPointLights[i].pos_att[3] = light->att;

				i++;
			}

			int pass = (LIGHT_VOLUME_BACK_FACE_PASS_2 - LIGHT_VOLUME_BACK_FACE_PASS_1) * j;

			graphics->renderer.submit((PassEnum)(LIGHT_VOLUME_BACK_FACE_PASS_1 + pass), LIGHT_INDEXING_TECH, &batch);
			graphics->renderer.submit((PassEnum)(LIGHT_VOLUME_FRONT_FACE_PASS_1 + pass), LIGHT_INDEXING_TECH, &batch);
			graphics->renderer.submit((PassEnum)(LIGHT_INDEXING_PASS_1 + pass), LIGHT_INDEXING_TECH, &batch);
		}
	}

	frameContext->numPointLights = i;

	batch.geom = (Geometry*)graphics->renderer.geomMgr->peekResource("screen-aligned quad");
	batch.textureInfo = 0;
	batch.instanceCount = 1;

	static pgn::Float3x2 I =
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	Movable2D* movable2D = (Movable2D*)cbufAllocator->alloc(sizeof(Movable2D), &batch.instanceCBlockBuf);
	movable2D->transf = I;
	movable2D->scaleX = 1.0f;
	movable2D->scaleY = 1.0f;

	for (int i = 0; i < graphics->renderer.cfg.numPostProcessingPasses; i++)
	{
		graphics->renderer.submit(graphics->renderer.cfg.postProcessingPasses[i], POST_PROCESSING_TECH, &batch);
	}

	Camera* camera = (Camera*)_camera;

	frameContext->view = camera->viewMat;
	frameContext->proj = camera->projMat;
	frameContext->viewport = camera->viewport;

	graphics->renderer.endSubmit();
}

void Scene::submitEntities(SceneEntityListItem* first, int count, CBufAllocator* cbufAllocator)
{
	assert(count <= maxInstanceCount);

	Entity* entity = first->sceneEntity->entity;
	Model* model = entity->model;

	if (!model->complete())
		return;

	model->submittingStamp = graphics->renderer.submittingCount;

	Batch batch;
	batch.geom = (Geometry*)model->geomHandle->core();
	batch.textureInfo = &model->textureInfo;

	TechEnum tech;

	pgn::SkeletonTemplate* skelTempl = batch.geom->skeletonTemplate;
	if (skelTempl)
	{
		tech = SKINNED_MESH_TECH;

		int numBones = skelTempl->getNumBones();
		pgn::Float4x3* boneMats = (pgn::Float4x3*)cbufAllocator->alloc(sizeof(entity->boneMats[0]) * numBones, &batch.boneMatBuf);

		if (entity->boneMats)
		{
			for (int j = 0; j < numBones; j++)
				boneMats[j] = entity->boneMats[j];
		}
		else
		{
			entity->skel->updatePose(0, skelTempl, boneMats);
		}
	}
	else
	{
		tech = STATIC_MESH_TECH;
	}

	SceneEntityListItem* item = first;
	int n = count;

	while (n)
	{
		batch.instanceCount = min(n, maxInstanceCount);
		Movable* instances = (Movable*)cbufAllocator->alloc(sizeof(Movable) * batch.instanceCount, &batch.instanceCBlockBuf);

		for (int i = 0; i < batch.instanceCount; i++)
		{
			instances[i] = item->sceneEntity->movable;
			item = item->next;
		}

		for (int i = 0; i < graphics->renderer.cfg.numOpaqueEntityPasses; i++)
		{
			graphics->renderer.submit(graphics->renderer.cfg.opaqueEntityPasses[i], tech, &batch);
		}

		n -= batch.instanceCount;
	}
}
