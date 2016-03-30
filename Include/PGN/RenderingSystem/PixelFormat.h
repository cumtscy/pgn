#pragma once
namespace pgn {

enum PixelFormat
{
	R8, RG8, RGB8, RGBA8,
	R8UI, RG8UI, RGB8UI, RGBA8UI,
	R16F, RG16F, RGB16F, RGBA16F,
	R32F, RG32F, reserved_for_RGB32F, reserved_for_RGBA32F,

	D24_S8, reserved_for_D32F_X24_S8, reserved_for_D32F,

	DXT1, DXT3, DXT5,
	RGB8_ETC2, RGB8_A1_ETC2, RGBA8_ETC2_EAC
};

}
