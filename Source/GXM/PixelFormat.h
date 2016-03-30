#include <gxm/constants.h>

struct PixelFormat
{
	SceGxmColorFormat colorFormat;
	SceGxmTextureFormat texFormat;
	bool compressed;
	int blockWidth;
	int blockHeight;
	int bitsPerPixel;
};

const SceGxmColorFormat nac = SCE_GXM_COLOR_FORMAT_U8_R;
const SceGxmTextureFormat nat = SCE_GXM_TEXTURE_FORMAT_U8_R;

const PixelFormat formats[] =
{
//	colorFormat									texFormat									compr	blockW	blockH	bpp
	SCE_GXM_COLOR_FORMAT_U8_R,					SCE_GXM_TEXTURE_FORMAT_U8_R,				0,		8,		1,		8,		// R8
	SCE_GXM_COLOR_FORMAT_U8U8_GR,				SCE_GXM_TEXTURE_FORMAT_U8U8_GR,				0,		8,		1,		16,		// RG8
	SCE_GXM_COLOR_FORMAT_U8U8U8_BGR,			SCE_GXM_TEXTURE_FORMAT_U8U8U8_BGR,			0,		8,		1,		24,		// RGB8
	SCE_GXM_COLOR_FORMAT_U8U8U8U8_ABGR,			SCE_GXM_TEXTURE_FORMAT_U8U8U8U8_ABGR,		0,		8,		1,		32,		// RGBA8
	SCE_GXM_COLOR_FORMAT_U8_R,					SCE_GXM_TEXTURE_FORMAT_U8_R,				0,		8,		1,		8,		// R8UI
	SCE_GXM_COLOR_FORMAT_U8U8_GR,				SCE_GXM_TEXTURE_FORMAT_U8U8_GR,				0,		8,		1,		16,		// RG8UI
	SCE_GXM_COLOR_FORMAT_U8U8U8_BGR,			SCE_GXM_TEXTURE_FORMAT_U8U8U8_BGR,			0,		8,		1,		24,		// RGB8UI
	SCE_GXM_COLOR_FORMAT_U8U8U8U8_ABGR,			SCE_GXM_TEXTURE_FORMAT_U8U8U8U8_ABGR,		0,		8,		1,		32,		// RGBA8UI
	SCE_GXM_COLOR_FORMAT_F16_R,					SCE_GXM_TEXTURE_FORMAT_F16_R,				0,		8,		1,		16,		// R16F
	SCE_GXM_COLOR_FORMAT_F16F16_GR,				SCE_GXM_TEXTURE_FORMAT_F16F16_GR,			0,		8,		1,		32,		// RG16F
	SCE_GXM_COLOR_FORMAT_F16F16F16F16_ABGR,		SCE_GXM_TEXTURE_FORMAT_X16F16F16F16_1BGR,	0,		8,		1,		64,		// RGB16F
	SCE_GXM_COLOR_FORMAT_F16F16F16F16_ABGR,		SCE_GXM_TEXTURE_FORMAT_F16F16F16F16_ABGR,	0,		8,		1,		64,		// RGBA16F
	SCE_GXM_COLOR_FORMAT_F32_R,					SCE_GXM_TEXTURE_FORMAT_F32_R,				0,		8,		1,		32,		// R32F
	SCE_GXM_COLOR_FORMAT_F32F32_GR,				SCE_GXM_TEXTURE_FORMAT_F32F32_GR,			0,		8,		1,		64,		// RG32F
	nac,										nat,										0,		8,		1,		96,		// reserved_for_RGB32F
	nac,										nat,										0,		8,		1,		128,	// reserved_for_RGBA32F

	nac,										SCE_GXM_TEXTURE_FORMAT_X8U24_SD,			0,		32,		1,		32,		// D24_S8
	nac,										nat,										0,		32,		1,		64,		// reserved_for_D32F_X24_S8
	nac,										nat,										0,		32,		1,		32,		// reserved_for_D32F

	nac,										SCE_GXM_TEXTURE_FORMAT_UBC1_ABGR,			1,		4,		4,		4,		// DXT1
	nac,										SCE_GXM_TEXTURE_FORMAT_UBC2_ABGR,			1,		4,		4,		8,		// DXT3
	nac,										SCE_GXM_TEXTURE_FORMAT_UBC3_ABGR,			1,		4,		4,		8,		// DXT5
	nac,										nat,										1,		4,		4,		4,		// RGB8_ETC2
	nac,										nat,										1,		4,		4,		4,		// RGB8_ETC2
	nac,										nat,										1,		4,		4,		8		// RGBA8_ETC2_EAC
};
