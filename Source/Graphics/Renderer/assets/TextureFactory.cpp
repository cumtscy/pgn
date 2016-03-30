#include <new>
#include <PGN/Utilities/Pool.h>
#include "Texture.h"
#include "TextureFactory.h"

TextureFactory::TextureFactory()
{
	pool = pgn::Pool::create(sizeof(_Texture));
}

TextureFactory::~TextureFactory()
{
	pool->destroy();
}

pgn::Asset* TextureFactory::create()
{
	return new(pool->alloc()) _Texture(this);
}

void TextureFactory::recycle(pgn::Asset* asset)
{
	pool->_free(asset);
}
