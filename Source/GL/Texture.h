#include <PGN/RenderingSystem/Texture.h>
#include "PixelFormat.h"

class Texture : public pgn::Texture
{
public:
	pgn::TextureDesc desc;
	unsigned tex;

	Texture(pgn::TextureDesc* desc, void* levels[]);
	virtual void dispose();
	virtual void _free();
	virtual pgn::TextureDesc* getDesc();
};
