#include <list>
#include <PGN/Math/Math.h>
#define PGN_STATIC_IMPORT
#include <PGN/Utilities/Pool.h>
#include <PGN/Utilities/PoolAllocator.h>
#undef PGN_STATIC_IMPORT
#include <PGN/Utilities/SkeletalAnimation/SkeletonTemplate.h>

struct Bone
{
	pgn::Float4x4 offsetMat;
	pgn::Float4 defaultRot;
	pgn::Float3 defaultPos;
	unsigned char parentIndex;
};

typedef pgn::PoolAllocator<Bone, pgn::Pool> BoneAllocator;

class SkeletonTemplate : public pgn::SkeletonTemplate
{
public:
	pgn::Pool* pool;
	std::list<Bone, BoneAllocator> bones;

	SkeletonTemplate(int numBones, pgn::Float4x3* offsetMats, pgn::Float4 defaultRot[], pgn::Float3 defaultPos[], unsigned char parentIndices[], pgn::Pool* pool, BoneAllocator& boneAllocator);
	virtual void dispose();
	virtual void _free();
	virtual int getNumBones();
};
