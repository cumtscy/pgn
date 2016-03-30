#pragma once
namespace pgn {

struct PNMHeader
{
	enum
	{
		  ATTRIB_POSITION
		, ATTRIB_NORMAL
		, ATTRIB_NUM_ASSIGNED_BONES
		, ATTRIB_BONE_INDICES
		, ATTRIB_BONE_WEIGHTS
		, ATTRIB_MATERIAL_INDEX
		, ATTRIB_TEX_COORD_0
		, ATTRIB_TEX_COORD_1

		, NUM_ATTRIBS
	};

	int numVerts;
	int attribChunkOffsetTable[NUM_ATTRIBS];

	static const int maxNumSubsets = 16;
	int numIndices[maxNumSubsets];
	int numSubsets;
	int indicesChunkOffset;

	int numBones;
	int offsetMatsChunkOffset;
	int defaultRotChunkOffset;
	int defaultPosChunkOffset;
	int parentIndicesChunkOffset;

	int materialChunkOffset;
	int materialChunkSize;
};

}
