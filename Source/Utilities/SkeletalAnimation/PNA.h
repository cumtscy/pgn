#define PGN_STATIC_IMPORT
#include <PGN/Utilities/ResourceManager/Asset.h>
#undef PGN_STATIC_IMPORT
#include "Anim.h"

class PNA : public pgn::Asset
{
	pgn::AssetFactory* factory;
	Anim anim;
	RotKeyAllocator& rotKeyAllocator;
	PosKeyAllocator& posKeyAllocator;
	bool ready;
public:
	PNA(pgn::AssetFactory* factory, BoneAnimAllocator& boneAnimAllocator, RotKeyAllocator& rotKeyAllocator, PosKeyAllocator& posKeyAllocator);
	virtual pgn::AssetFactory* getFactory();
	virtual bool cook(void* rawData);
	virtual bool submit(void* rawData, void* customArg);
	virtual void unload(void* customArg);
	virtual void* core();
};
