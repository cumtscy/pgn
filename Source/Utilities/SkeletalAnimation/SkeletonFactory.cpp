#define PGN_DLL_EXPORT
#include <PGN/Utilities/SkeletalAnimation/SkeletonFactory.h>
#undef PGN_DLL_EXPORT

#include <PGN/Common/debug_new.h>
#include <new>
#define PGN_STATIC_IMPORT
#include <PGN/Utilities/Pool.h>
#undef PGN_STATIC_IMPORT
#include "Skeleton.h"

class SkeletonFactory : public pgn::SkeletonFactory
{
	pgn::Pool* pool;
public:
	SkeletonFactory();
	virtual void dispose();
	virtual void _free();
	virtual pgn::Skeleton* createSkeleton();
};

SkeletonFactory::SkeletonFactory()
{
	pool = pgn::Pool::create(sizeof(Skeleton));
}

void SkeletonFactory::dispose()
{
	pool->destroy();
}

pgn::SkeletonFactory* pgn::SkeletonFactory::create()
{
	return debug_new ::SkeletonFactory;
}

void SkeletonFactory::_free()
{
	delete this;
}

pgn::Skeleton* SkeletonFactory::createSkeleton()
{
	return new(pool->alloc()) Skeleton(pool);
}

void Skeleton::_free()
{
	pool->_free(this);
}
