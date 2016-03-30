#include <cassert>
#include <gxm/texture.h>
#include <PGN/Common/debug_new.h>
#include <PGN/Utilities/ManagedHeap.h>
#include <string.h>
#include "PixelFormat.h"
#include "RenderingSystem.h"
#include "Sampler.h"
#include "Texture.h"

#define align(x,a) ((x + ((a)-1)) & ~((a)-1))

Texture::Texture(pgn::TextureDesc* desc, void* levels[], pgn::ManagedHeap* heap)
{
	this->desc = *desc;
	this->heap = heap;

	const PixelFormat* format = &formats[desc->format];

	int size = 0;

	int width = desc->width;
	int height = desc->height;

	for (int i = 0; i < desc->numLevels; i++)
	{
		size += format->bitsPerPixel * align(width, format->blockWidth) * align(height, format->blockHeight) / 8;

		if (width > 1) width /= 2;
		if (height > 1) height /= 2;
	}

	assert(!(size % videoMemAlign));
	memBlock = heap->alloc(size);
	memcpy(memBlock->p, levels[0], size); // todo: ÓÅ»¯¡£

	if (format->compressed)
		sceGxmTextureInitSwizzled(&tex, memBlock->p, format->texFormat, desc->width, desc->height, desc->numLevels);
	else
		sceGxmTextureInitLinear(&tex, memBlock->p, format->texFormat, desc->width, desc->height, desc->numLevels);

	tex.controlWords[0] &= samplerClearMask;
}

void Texture::dispose()
{
	heap->_free(memBlock);
}

pgn::Texture* RenderingSystem::createTexture(pgn::TextureDesc* desc, void* levels[])
{
	return debug_new Texture(desc, levels, videoMemHeap);
}

void Texture::_free()
{
	delete this;
}

pgn::TextureDesc* Texture::getDesc()
{
	return &desc;
}

void RenderingSystem::setTextures(int startSlot, int numTextures, pgn::Texture* textures[])
{
	unsigned textureClearMask = ~samplerClearMask;

	for (int i = 0; i < numTextures; i++)
	{
		int textureIndex = startSlot + i;

		SceGxmTexture* texture = &this->textures[textureIndex];
		SceGxmTexture* tex = &((Texture*)textures[i])->tex;

		unsigned controlWord = texture->controlWords[0] & textureClearMask;

		texture->controlWords[0] = controlWord | tex->controlWords[0];
		texture->controlWords[1] = tex->controlWords[1];
		texture->controlWords[2] = tex->controlWords[2];
		texture->controlWords[3] = tex->controlWords[3];
	}
}
