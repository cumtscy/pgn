#include <PGN/Common/debug_new.h>
#include "RenderingSystem.h"
#include "ResourceView.h"

pgn::DepthStencilView* RenderingSystem::createDepthStencilView(pgn::RenderBuffer* rb)
{
	return debug_new DepthStencilView((RenderBuffer*)rb);
}

pgn::DepthStencilView* RenderingSystem::createDepthStencilView(pgn::Texture* tex)
{
	return debug_new DepthStencilView((Texture*)tex);
}

pgn::RenderTargetView* RenderingSystem::createRenderTargetView(pgn::RenderBuffer* rb)
{
	return debug_new RenderTargetView((RenderBuffer*)rb);
}

pgn::RenderTargetView* RenderingSystem::createRenderTargetView(pgn::Texture* tex)
{
	return debug_new RenderTargetView((Texture*)tex);
}
