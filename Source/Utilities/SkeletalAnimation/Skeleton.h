#include <PGN/Utilities/SkeletalAnimation/Skeleton.h>

namespace pgn {
	class ResourceHandle;
	class Pool;
}

class SkeletonTemplate;

struct AnimLayer
{
	pgn::ResourceHandle* anim;
	int transitionTime;
	pgn::TransitionFunc transitionFunc;
	float rate;
	int startTime;

	int time;
};

class Skeleton : public pgn::Skeleton
{
	pgn::Pool* pool;
	static const int maxNumAnimLayers = 8;
	AnimLayer animLayers[maxNumAnimLayers];
	int numAnimLayers;
public:
	Skeleton(pgn::Pool* pool);
	virtual void dispose();
	virtual void _free();
	virtual void playAnimation(pgn::Animation* anim, int transitionTime, pgn::TransitionFunc transitionFunc, float rate, int startTime);
	virtual void updatePose(int dt, pgn::SkeletonTemplate* templ, pgn::Float4x3* boneMats);
};
