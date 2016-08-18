#define PGN_DLL_EXPORT
#include <PGN/FatMan/SkeletalAnimation/SkeletonTemplateFactory.h>
#undef PGN_DLL_EXPORT

#include <PGN/Common/debug_new.h>
#include <new>
#include "SkeletonTemplate.h"

class SkeletonTemplateFactory : public pgn::SkeletonTemplateFactory
{
	pgn::Pool* skeletonTemplatePool;
	BoneAllocator boneAllocator;

public:
	SkeletonTemplateFactory();
	virtual void dispose();
	virtual void _free();
	virtual pgn::SkeletonTemplate* createSkeletonTemplate(int numBones, pgn::Float4x3* offsetMats, pgn::Float4 defaultRot[], pgn::Float4 defaultPos[], unsigned char parentIndices[]);
};

SkeletonTemplateFactory::SkeletonTemplateFactory()
{
	skeletonTemplatePool = pgn::Pool::create(sizeof(SkeletonTemplate));
}

void SkeletonTemplateFactory::dispose()
{
	skeletonTemplatePool->destroy();
}

pgn::SkeletonTemplateFactory* pgn::SkeletonTemplateFactory::create()
{
	return debug_new ::SkeletonTemplateFactory;
}

void SkeletonTemplateFactory::_free()
{
	delete this;
}

pgn::SkeletonTemplate* SkeletonTemplateFactory::createSkeletonTemplate(int numBones, pgn::Float4x3* offsetMats, pgn::Float4 defaultRot[], pgn::Float4 defaultPos[], unsigned char parentIndices[])
{
	return new(skeletonTemplatePool->alloc()) SkeletonTemplate(numBones, offsetMats, defaultRot, defaultPos, parentIndices, skeletonTemplatePool, boneAllocator);
}

void SkeletonTemplate::_free()
{
	this->~SkeletonTemplate();
	pool->_free(this);
}
