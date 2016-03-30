#include <PGN/Utilities/ResourceManager/AssetFactory.h>
#include "../Geometry.h"

namespace pgn {
	class SkeletonTemplateFactory;
	class Pool;
}

class MeshFactory : public pgn::AssetFactory
{
public:
	pgn::Pool* pool;
	SubsetAllocator& subsetAllocator;
	pgn::SkeletonTemplateFactory* skeletonTemplateFactory;

	MeshFactory(SubsetAllocator& subsetAllocator);
	~MeshFactory();
	virtual pgn::Asset* create();
	virtual void recycle(pgn::Asset* asset);
};
