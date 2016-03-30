#include <PGN/Assets/PNT.h>
#include <PGN/RenderingSystem/RenderingSystem.h>
#include <PGN/RenderingSystem/Texture.h>
#include "Texture.h"

_Texture::_Texture(pgn::AssetFactory* factory)
	: Asset(factory)
{
	tex = 0;
}

bool _Texture::cook(void* rawData)
{
	return true;
}

bool _Texture::submit(void* rawData, void* customArg)
{
	pgn::RenderingSystem* rs = (pgn::RenderingSystem*)customArg;
	pgn::PNTHeader* header = (pgn::PNTHeader*)rawData;

	static pgn::PixelFormat internalFormatLookupTable[] =
	{
		pgn::R8, pgn::RG8, pgn::RGB8, pgn::RGBA8,
		pgn::R16F, pgn::RG16F, pgn::RGB16F, pgn::RGBA16F,
		pgn::R32F, pgn::RG32F, pgn::reserved_for_RGB32F, pgn::reserved_for_RGBA32F,

		pgn::DXT1, pgn::DXT3, pgn::DXT5,
		pgn::RGB8_ETC2, pgn::RGB8_A1_ETC2, pgn::RGBA8_ETC2_EAC
	};

	pgn::TextureDesc texDesc;
	texDesc.format = internalFormatLookupTable[header->format];
	texDesc.width = header->width;
	texDesc.height = header->height;
	texDesc.numLevels = header->numLevels;

	const int maxNumLevels = 16;
	void* levels[maxNumLevels];

	for (int i = 0; i < texDesc.numLevels; i++)
		levels[i] = (char*)header + header->levelOffsets[i];

	tex = rs->createTexture(&texDesc, levels);

	rs->flush();
	return true;
}

void _Texture::unload(void* customArg)
{
	if (!tex)
		return;

	tex->destroy();
}

void* _Texture::core()
{
	return tex;
}
