#pragma once
#include <list>
#include <PGN/RenderingSystem/RenderingSystem.h>
#include <PGN/Utilities/Pool.h>
#include <PGN/Utilities/PoolAllocator.h>
#include "CBufRange.h"
#include "VertexFormat.h"

namespace pgn {
	class SkeletonTemplate;
}

struct Subset
{
	int indexBufferOffset;
	int numIndices;

	Subset(int indexBufferOffset, int numIndices)
		: indexBufferOffset(indexBufferOffset)
		, numIndices(numIndices)
	{}
};

typedef pgn::PoolAllocator<Subset, pgn::Pool> SubsetAllocator;

class Geometry
{
public:
	pgn::PrimType						primType;
	const VertexFormat*					vertexFormat;
	pgn::Buffer*						vertexBuffers[VertexFormat::maxNumAttribs];
	int									strides[VertexFormat::maxNumAttribs];
	int									offsets[VertexFormat::maxNumAttribs];
	pgn::Buffer*						indexBuffer;
	std::list<Subset, SubsetAllocator>	subsets;
	CBufRange							brdfCoeffBuf;
	pgn::SkeletonTemplate*				skeletonTemplate;

	Geometry(SubsetAllocator& subsetAllocator)
		: subsets(subsetAllocator)
	{}
};
