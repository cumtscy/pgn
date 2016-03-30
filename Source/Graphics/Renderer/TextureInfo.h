#pragma once
#include <list>
#include <PGN/Utilities/Pool.h>
#include <PGN/Utilities/PoolAllocator.h>

namespace pgn {
	class ResourceHandle;
	class Texture;
}

const int maxTexturesPerSubset = 4;

struct TextureSet
{
	pgn::ResourceHandle* textures[maxTexturesPerSubset];

	TextureSet()
	{
		for (auto& tex : textures)
			tex = 0;
	}
};

typedef pgn::PoolAllocator<TextureSet, pgn::Pool> TextureSetAllocator;

class TextureInfo
{
public:
	pgn::Texture** texBindingPoints[maxTexturesPerSubset];
	std::list<TextureSet, TextureSetAllocator> textureSets;

	TextureInfo(TextureSetAllocator& texSetAllocator)
		: textureSets(texSetAllocator)
	{}
};
