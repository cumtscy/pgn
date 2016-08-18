#include <new>
#include <PGN/FatMan/SkeletalAnimation/SkeletonTemplateFactory.h>
#include <PGN/Utilities/Pool.h>
#include "Mesh.h"
#include "MeshFactory.h"

MeshFactory::MeshFactory(SubsetAllocator& subsetAllocator)
	: subsetAllocator(subsetAllocator)
{
	pool = pgn::Pool::create(sizeof(Mesh));
	skeletonTemplateFactory = pgn::SkeletonTemplateFactory::create();
}

MeshFactory::~MeshFactory()
{
	pool->destroy();
	skeletonTemplateFactory->destroy();
}

pgn::Asset* MeshFactory::create()
{
	return new(pool->alloc()) Mesh(this, subsetAllocator, skeletonTemplateFactory);
}

void MeshFactory::recycle(pgn::Asset* asset)
{
	((Mesh*)asset)->~Mesh();
	pool->_free(asset);
}
