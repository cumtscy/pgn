#include <cassert>
#include <PGN/Common/debug_new.h>
#include <PGN/Utilities/ManagedHeap.h>
#include "PixelFormat.h"
#include "RenderBuffer.h"
#include "RenderingSystem.h"

#define align(x,a) ((x + ((a)-1)) & ~((a)-1))

RenderBuffer::RenderBuffer(pgn::RenderBufferDesc* desc, pgn::ManagedHeap* heap)
{
	format = desc->format;

	width = desc->width;
	height = desc->height;

	samples = desc->samples;

	this->heap = heap;

	int sizeBuf;

	if (format == pgn::D24_S8)
	{
		if (desc->samples == 4)
		{
			strideInSamples = align(width * 2, SCE_GXM_TILE_SIZEX);
			sizeBuf = 4 * strideInSamples * height * 2;
		}
		else if (desc->samples == 2)
		{
			strideInSamples = align(width, SCE_GXM_TILE_SIZEX);
			sizeBuf = 4 * strideInSamples * height * 2;
		}
		else// if (desc->samples == 1)
		{
			strideInSamples = align(width, SCE_GXM_TILE_SIZEX);
			sizeBuf = 4 * strideInSamples * height;
		}
	}
	else
	{
		strideInPixels = align(width, 2);
		sizeBuf = formats[format].bitsPerPixel / 8 * strideInPixels * height;
	}

	assert(!(sizeBuf % videoMemAlign));
	memBlock = heap->alloc(sizeBuf);
}

void RenderBuffer::dispose()
{
	heap->_free(memBlock);
}

pgn::RenderBuffer* RenderingSystem::createRenderBuffer(pgn::RenderBufferDesc* desc)
{
	return debug_new RenderBuffer(desc, videoMemHeap);
}

void RenderBuffer::_free()
{
	delete this;
}
