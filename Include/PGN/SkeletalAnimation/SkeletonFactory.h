#pragma once
#include "../Common/DllInterface.h"
#include "../Common/Interface.h"
namespace pgn {

class Skeleton;
class SkeletonTemplate;

class SkeletonFactory : public Interface
{
public:
	static PGN_DLL_INTERFACE SkeletonFactory* create();

	virtual Skeleton* createSkeleton() = 0;
};

}
