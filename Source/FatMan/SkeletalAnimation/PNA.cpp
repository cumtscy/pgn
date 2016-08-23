#include <climits>
#include <PGN/Assets/PNA.h>
#include "PNA.h"

PNA::PNA(pgn::AssetFactory* factory, BoneAnimAllocator& boneAnimAllocator, RotKeyAllocator& rotKeyAllocator, PosKeyAllocator& posKeyAllocator)
	: anim(boneAnimAllocator)
	, rotKeyAllocator(rotKeyAllocator)
	, posKeyAllocator(posKeyAllocator)
{
	this->factory = factory;
	ready = false;
}

pgn::AssetFactory* PNA::getFactory()
{
	return factory;
}

bool PNA::cook(void* rawData)
{
	pgn::PNAHeader* header = (pgn::PNAHeader*)rawData;
	anim.duration = 0;

	for (int i = 0; i < header->numBones; i++)
	{
		anim.boneAnims.emplace_back(rotKeyAllocator, posKeyAllocator);
		BoneAnim* boneAnim = &anim.boneAnims.back();
		pgn::PNAHeader::BoneAnimation* rawBoneAnim = &header->boneAnimations[i];

		int numRotKeys = rawBoneAnim->numRotKeys;
		int* rotKeyTimes = (int*)((char*)rawData + rawBoneAnim->rotKeyTimesChunkOffset);
		pgn::Float4* rotKeys = (pgn::Float4*)((char*)rawData + rawBoneAnim->rotKeysChunkOffset);

		for (int j = 0; j < numRotKeys; j++)
			boneAnim->rotKeyMap[rotKeyTimes[j]] = rotKeys[j];

		if (numRotKeys)
		{
			boneAnim->rotKeyMap[INT_MAX] = rotKeys[numRotKeys - 1];
			int endTime = rotKeyTimes[numRotKeys - 1];
			if (endTime > anim.duration)
				anim.duration = endTime;
		}

		int numPosKeys = rawBoneAnim->numPosKeys;
		int* posKeyTimes = (int*)((char*)rawData + rawBoneAnim->posKeyTimesChunkOffset);
		pgn::Float3* posKeys = (pgn::Float3*)((char*)rawData + rawBoneAnim->posKeysChunkOffset);

		for (int j = 0; j < numPosKeys; j++)
			boneAnim->posKeyMap[posKeyTimes[j]] = posKeys[j];

		if (numPosKeys)
		{
			boneAnim->posKeyMap[INT_MAX] = posKeys[numPosKeys - 1];
			int endTime = posKeyTimes[numPosKeys - 1];
			if (endTime > anim.duration)
				anim.duration = endTime;
		}
	}

	ready = true;
	return true;
}

bool PNA::submit(void* rawData, void* customArg)
{
	return true;
}

void PNA::unload(void* customArg)
{
	if (!ready)
		return;

	anim.boneAnims.clear();
}

void* PNA::core()
{
	return ready ? &anim : 0;
}
