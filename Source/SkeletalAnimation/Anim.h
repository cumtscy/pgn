#pragma once
#include <list>
#include <map>
#include <PGN/Math/Math.h>
#include <PGN/Utilities/Pool.h>
#include <PGN/Utilities/PoolAllocator.h>

typedef pgn::PoolAllocator<std::pair<int, pgn::Float4>, pgn::Pool> RotKeyAllocator;
typedef pgn::PoolAllocator<std::pair<int, pgn::Float4>, pgn::Pool> PosKeyAllocator;

class BoneAnim
{
public:
	std::map<int, pgn::Float4, std::less<int>, RotKeyAllocator> rotKeyMap;
	std::map<int, pgn::Float4, std::less<int>, PosKeyAllocator> posKeyMap;

	BoneAnim(RotKeyAllocator& rotKeyAllocator, PosKeyAllocator& posKeyAllocator)
		: rotKeyMap(rotKeyAllocator)
		, posKeyMap(posKeyAllocator)
	{}
};

typedef pgn::PoolAllocator<BoneAnim, pgn::Pool> BoneAnimAllocator;

class Anim
{
public:
	std::list<BoneAnim, BoneAnimAllocator> boneAnims;
	int duration;

	Anim(BoneAnimAllocator& boneAnimAllocator)
		: boneAnims(boneAnimAllocator)
	{}
};
