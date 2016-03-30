#include <PGN/Utilities/ResourceManager/AssetFactory.h>
#include "Anim.h"

namespace pgn {
	class Pool;
}

class PNAFactory : public pgn::AssetFactory
{
public:
	pgn::Pool* pool;
	BoneAnimAllocator boneAnimAllocator;
	RotKeyAllocator rotKeyAllocator;
	PosKeyAllocator posKeyAllocator;

	PNAFactory();
	~PNAFactory();
	virtual pgn::Asset* create();
	virtual void recycle(pgn::Asset* asset);
};
