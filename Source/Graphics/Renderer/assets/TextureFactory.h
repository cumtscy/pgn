#include <PGN/Utilities/ResourceManager/AssetFactory.h>

namespace pgn {
	class Pool;
}

class TextureFactory : public pgn::AssetFactory
{
public:
	pgn::Pool* pool;

	TextureFactory();
	~TextureFactory();
	virtual pgn::Asset* create();
	virtual void recycle(pgn::Asset* asset);
};
