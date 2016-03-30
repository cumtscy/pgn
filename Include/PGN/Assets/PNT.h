#pragma once
namespace pgn {

struct PNTHeader
{
	enum Format
	{
		R8, RG8, RGB8, RGBA8,
		R16F, RG16F, RGB16F, RGBA16F,
		R32F, RG32F, RGB32F, RGBA32F,

		DXT1, DXT3, DXT5,
		RGB8_ETC2, RGB8_A1_ETC2, RGBA8_ETC2_EAC
	};

	Format format;
	int width;
	int height;
	int numLevels;
	int levelOffsets[16];
};

}
