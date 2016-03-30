#pragma once
namespace pgn {

struct PNAHeader
{
	struct BoneAnimation
	{
		int numRotKeys;
		int rotKeyTimesChunkOffset;
		int rotKeysChunkOffset;

		int numPosKeys;
		int posKeyTimesChunkOffset;
		int posKeysChunkOffset;
	};

	int numBones;
	BoneAnimation boneAnimations[1];
};

}
