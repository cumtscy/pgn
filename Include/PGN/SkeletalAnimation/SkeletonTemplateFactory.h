#pragma once
#include "../Common/DllInterface.h"
#include "../Common/Interface.h"
namespace pgn {

struct Float4x3;
class SkeletonTemplate;
struct Float4;

class SkeletonTemplateFactory : public Interface
{
public:
	static PGN_DLL_INTERFACE SkeletonTemplateFactory* create();

	virtual SkeletonTemplate* createSkeletonTemplate(int numBones, Float4x3* offsetMats, Float4 defaultRot[], Float4 defaultPos[], unsigned char parentIndices[]) = 0;
};

}
