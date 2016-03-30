#pragma once
#include "../Common/Interface.h"
namespace pgn {

class Animation;
struct Float4x3;
class SkeletonTemplate;

enum TransitionFunc
{
	  linear
	, easeIn
	, easeOut
	, easeInOut
};

class Skeleton : public Interface
{
public:
	virtual void playAnimation(Animation* anim, int transitionTime = 0, TransitionFunc transitionFunc = linear, float rate = 1.0f/*负数就是倒着播*/, int startTime = 0) = 0;
	virtual void updatePose(int dt, SkeletonTemplate* templ = 0, Float4x3* boneMats = 0) = 0;
};

}
