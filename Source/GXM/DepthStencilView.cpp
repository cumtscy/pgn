#include <gxm/surface.h>
#include <PGN/Common/debug_new.h>
#include <PGN/Utilities/ManagedHeap.h>
#include "DepthStencilView.h"
#include "PixelFormat.h"
#include "RenderingSystem.h"

#define align(x,a) ((x + ((a)-1)) & ~((a)-1))

DepthStencilView::DepthStencilView(RenderBuffer* rb)
	: BaseDepthStencilView(rb)
{
	sceGxmDepthStencilSurfaceInit(
		&depthStencilSurf,
		SCE_GXM_DEPTH_STENCIL_FORMAT_S8D24,
		SCE_GXM_DEPTH_STENCIL_SURFACE_LINEAR,
		rb->strideInSamples,
		rb->memBlock->p,
		0);
}

DepthStencilView::DepthStencilView(Texture* tex)
	: BaseDepthStencilView(tex)
{
	const PixelFormat* format = &formats[tex->desc.format];

	sceGxmDepthStencilSurfaceInit(
		&depthStencilSurf,
		SCE_GXM_DEPTH_STENCIL_FORMAT_S8D24,
		SCE_GXM_DEPTH_STENCIL_SURFACE_LINEAR,
		align(tex->desc.width, format->blockWidth),
		tex->memBlock->p,
		0);
}

pgn::DepthStencilView* RenderingSystem::createDepthStencilView(pgn::RenderBuffer* rb)
{
	return debug_new DepthStencilView((RenderBuffer*)rb);
}

pgn::DepthStencilView* RenderingSystem::createDepthStencilView(pgn::Texture* tex)
{
	return debug_new DepthStencilView((Texture*)tex);
}
