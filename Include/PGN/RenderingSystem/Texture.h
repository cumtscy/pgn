#pragma once
#include "../Common/Interface.h"
#include "PixelFormat.h"
namespace pgn {

struct TextureDesc
{
	PixelFormat format;
	int width;
	int height;
	int numLevels;
};

class Texture : public Interface
{
public:
	virtual TextureDesc* getDesc() = 0;
};

}
