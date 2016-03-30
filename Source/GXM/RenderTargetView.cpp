#include <gxm/surface.h>
#include <PGN/Common/debug_new.h>
#include <PGN/Utilities/ManagedHeap.h>
#include "PixelFormat.h"
#include "RenderingSystem.h"
#include "RenderTargetView.h"

#define align(x,a) ((x + ((a)-1)) & ~((a)-1))

RenderTargetView::RenderTargetView(RenderBuffer* rb)
	: BaseRenderTargetView(rb)
{
	const PixelFormat* format = &formats[rb->format];

	sceGxmColorSurfaceInit(
		&colorSurf,
		format->colorFormat,
		SCE_GXM_COLOR_SURFACE_LINEAR,
		rb->samples > 1 ? SCE_GXM_COLOR_SURFACE_SCALE_MSAA_DOWNSCALE : SCE_GXM_COLOR_SURFACE_SCALE_NONE,
		format->bitsPerPixel > 32 ? SCE_GXM_OUTPUT_REGISTER_SIZE_64BIT : SCE_GXM_OUTPUT_REGISTER_SIZE_32BIT,
		rb->width,
		rb->height,
		rb->strideInPixels,
		rb->memBlock->p);
}

RenderTargetView::RenderTargetView(Texture* tex)
	: BaseRenderTargetView(tex)
{
	const PixelFormat* format = &formats[tex->desc.format];

	sceGxmColorSurfaceInit(
		&colorSurf,
		format->colorFormat,
		SCE_GXM_COLOR_SURFACE_LINEAR,
		SCE_GXM_COLOR_SURFACE_SCALE_NONE,
		format->bitsPerPixel > 32 ? SCE_GXM_OUTPUT_REGISTER_SIZE_64BIT : SCE_GXM_OUTPUT_REGISTER_SIZE_32BIT,
		tex->desc.width,
		tex->desc.height,
		align(tex->desc.width, format->blockWidth),
		tex->memBlock->p);
}

pgn::RenderTargetView* RenderingSystem::createRenderTargetView(pgn::RenderBuffer* rb)
{
	return debug_new RenderTargetView((RenderBuffer*)rb);
}

pgn::RenderTargetView* RenderingSystem::createRenderTargetView(pgn::Texture* tex)
{
	return debug_new RenderTargetView((Texture*)tex);
}
