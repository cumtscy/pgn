#pragma once
#include "../Common/Interface.h"
#include "PixelFormat.h"
namespace pgn {

struct RenderBufferDesc
{
	PixelFormat format;
	int width;
	int height;
	int samples;

	RenderBufferDesc()
		: samples(1)
	{
	}
};

class RenderBuffer : public Interface
{
public:
};

}
