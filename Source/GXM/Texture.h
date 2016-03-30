#include <gxm/structs.h>
#include <PGN/RenderingSystem/Texture.h>

namespace pgn {
	class ManagedHeap;
	class MemBlock;
}

class Texture : public pgn::Texture
{
public:
	pgn::TextureDesc desc;

	pgn::ManagedHeap* heap;
	pgn::MemBlock* memBlock;

	SceGxmTexture tex;

	Texture(pgn::TextureDesc* desc, void* levels[], pgn::ManagedHeap* heap);
	virtual void dispose();
	virtual void _free();
	virtual pgn::TextureDesc* getDesc();
};
