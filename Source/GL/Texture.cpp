#include <PGN/Common/debug_new.h>
#include "RenderingSystem.h"
#include "Texture.h"

#ifndef max
#define max(a,b) (a > b ? a : b)
#endif

Texture::Texture(pgn::TextureDesc* desc, void* levels[])
{
	this->desc = *desc;
	const PixelFormat* format = &formats[desc->format];

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	int width = desc->width;
	int height = desc->height;

	for (int i = 0; i < desc->numLevels; i++)
	{
		if(format->compressed)
		{
			int size = format->bitsPerPixel * max(width, format->blockWidth) * max(height, format->blockHeight) / 8;
			glCompressedTexImage2D(GL_TEXTURE_2D, i, format->internalFormat, width, height, 0, size, levels ? levels[i] : 0);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, i, format->internalFormat, width, height, 0, format->format, format->type, levels ? levels[i] : 0);
		}
		if(width > 1) width /= 2;
		if(height > 1) height /= 2;
	}
}

void Texture::dispose()
{
	glDeleteTextures(1, &tex);
}

pgn::Texture* RenderingSystem::createTexture(pgn::TextureDesc* desc, void* levels[])
{
	return debug_new Texture(desc, levels);
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
	for(int i = 0; i < numTextures; i++)
	{
		int indexTextureUnit = startSlot + i;
		glActiveTexture(GL_TEXTURE0 + indexTextureUnit);
		glBindTexture(GL_TEXTURE_2D, textures[i] ? ((Texture*)textures[i])->tex : 0);
	}
}
