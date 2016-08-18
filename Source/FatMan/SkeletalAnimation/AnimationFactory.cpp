#define PGN_DLL_EXPORT
#include <PGN/FatMan/SkeletalAnimation/AnimationFactory.h>
#undef PGN_DLL_EXPORT

#include <PGN/Common/debug_new.h>
#include <PGN/Utilities/ResourceManager/AsyncLoader.h>
#include <PGN/Utilities/ResourceManager/ResourceManager.h>
#include "Animation.h"
#include "PNAFactory.h"

class AnimationFactory : public pgn::AnimationFactory
{
	pgn::Pool* pool;
	PNAFactory* pnaFactory;
	pgn::ResourceManager* animMgr;
public:
	AnimationFactory(pgn::FileStream* f);
	virtual void dispose();
	virtual void _free();
	virtual pgn::Animation* createAnimation();
};

AnimationFactory::AnimationFactory(pgn::FileStream* f)
{
	pool = pgn::Pool::create(sizeof(Animation));
	pnaFactory = debug_new PNAFactory;
	animMgr = pgn::ResourceManager::create(pnaFactory, f, pgn::getDefaultAsyncLoader());
}

void AnimationFactory::dispose()
{
	animMgr->destroy();
	delete pnaFactory;
	pgn::releaseDefaultAsyncLoader();
	pool->destroy();
}

pgn::AnimationFactory* pgn::AnimationFactory::create(FileStream* f)
{
	return debug_new ::AnimationFactory(f);
}

void AnimationFactory::_free()
{
	delete this;
}

pgn::Animation* AnimationFactory::createAnimation()
{
	return new(pool->alloc()) Animation(pool, animMgr);
}

void Animation::_free()
{
	pool->_free(this);
}
