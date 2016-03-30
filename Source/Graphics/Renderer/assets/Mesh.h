#include "../Geometry.h"
#include "Asset.h"

namespace pgn {
	class SkeletonTemplateFactory;
}

class Mesh : public Asset
{
public:
	Geometry geom;
	pgn::SkeletonTemplateFactory* skeletonFactory;
	bool ready;

	Mesh(pgn::AssetFactory* factory, SubsetAllocator& subsetAllocator, pgn::SkeletonTemplateFactory* skeletonFactory);
	virtual bool cook(void* rawData);
	virtual bool submit(void* rawData, void* customArg);
	virtual void unload(void* customArg);
	virtual void* core();
};
