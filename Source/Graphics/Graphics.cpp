#include <PGN/Assets/PNM.h>
#include <PGN/Common/debug_new.h>
#include <PGN/Utilities/Heap.h>
#include "Entity.h"
#include "Graphics.h"
#include "Model.h"
#include "PointLight.h"

Graphics::Graphics(pgn::Display displayPrototype, pgn::FileStream* assetStream, pgn::FileStream* cacheStream)
	: renderer(displayPrototype, assetStream, cacheStream)
{
	modelPool = pgn::Pool::create(sizeof(Model));
	entityPool = pgn::Pool::create(sizeof(Entity));
	pointLightPool = pgn::Pool::create(sizeof(PointLight));
	tmpBuf = pgn::Heap::create();
}

void Graphics::dispose()
{
	modelPool->destroy();
	entityPool->destroy();
	pointLightPool->destroy();
	tmpBuf->destroy();
}

pgn::Graphics* pgn::Graphics::create(pgn::Display displayPrototype, pgn::FileStream* assetStream, pgn::FileStream* cacheStream)
{
	return debug_new ::Graphics(displayPrototype, assetStream, cacheStream);
}

void Graphics::_free()
{
	delete this;
}

void Graphics::beginDraw(pgn::Window* wnd)
{
	renderer.beginDraw(wnd, &lightIndexedForwardRendering);
}

void Graphics::endDraw()
{
	renderer.finish();
	performPendingRemovals();
	renderer.endDraw();
}

void Graphics::performPendingRemovals()
{
	while (!pendingRemovals.empty())
	{
		Model* model = pendingRemovals.front();

		if (model->submittingStamp > renderer.finishCount) break;

		model->~Model();
		modelPool->_free(model);
		pendingRemovals.pop_front();
	}
}
