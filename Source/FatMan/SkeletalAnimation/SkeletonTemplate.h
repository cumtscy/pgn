#include <list>
#include <PGN/FatMan/SkeletalAnimation/SkeletonTemplate.h>
#include <PGN/Math/Math.h>
#include <PGN/Utilities/Pool.h>
#include <PGN/Utilities/PoolAllocator.h>

struct Bone
{
	pgn::Float4x4 offsetMat;
	pgn::Float4 defaultRot;
	pgn::Float4 defaultPos;
	unsigned char parentIndex;
};

typedef pgn::PoolAllocator<Bone, pgn::Pool> BoneAllocator;

class SkeletonTemplate : public pgn::SkeletonTemplate
{
public:
	pgn::Pool* pool;
	std::list<Bone, BoneAllocator> bones;

	SkeletonTemplate(int numBones, pgn::Float4x3* offsetMats, pgn::Float4 defaultRot[], pgn::Float4 defaultPos[], unsigned char parentIndices[], pgn::Pool* pool, BoneAllocator& boneAllocator);
	virtual void dispose();
	virtual void _free();
	virtual int getNumBones();
};
