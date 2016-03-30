#include <PGN/Common/debug_new.h>
#include <PGN/RenderingSystem/Buffer.h>
#include <PGN/RenderingSystem/RenderingSystem.h>
#include <PGN/Utilities/Pool.h>
#include "Geometry.h"
#include "GeometryHelper.h"

GeometryHelper::GeometryHelper(pgn::RenderingSystem* rs)
{
	this->rs = rs;
}

void GeometryHelper::createGeometry(Geometry* geom, int numAttribs, pgn::VertexAttribDesc attribDescs[], int numVerts, int numSubsets, int numIndices[], void* attribs[], void* indices)
{
	static int sizeOf[] =
	{
		4, 8, 12, 16,	// float
		2, 4, 6, 8,		// half
		1, 2, 3, 4,		// byte
		1, 2, 3, 4,		// ubyte
		2, 4, 6, 8,		// short
		2, 4, 6, 8,		// ushort
	};

	pgn::BufferDesc vbDesc;
	vbDesc.bindingType	= pgn::VERTEX_BUFFER;
	vbDesc.usage		= pgn::STATIC_DRAW;

	for(int i = 0; i < numAttribs; i++)
	{
		vbDesc.size = sizeOf[attribDescs[i].format] * numVerts;

		pgn::Buffer* buf = rs->createBuffer(&vbDesc);

		geom->vertexBuffers[i] = buf;
		geom->strides[i] = sizeOf[attribDescs[i].format];
		geom->offsets[i] = 0;

		if(attribs)
			buf->update(0, vbDesc.size, attribs[i]);
	}

	geom->vertexFormat = chooseVertexFormat(numAttribs, attribDescs, geom->strides);

	int indexBufferSize = 0;

	for(int i = 0; i < numSubsets; i++)
	{
		geom->subsets.emplace_back(indexBufferSize, numIndices[i]);
		indexBufferSize += sizeof(unsigned short)* numIndices[i];
	}

	pgn::BufferDesc ibDesc;
	ibDesc.bindingType	= pgn::INDEX_BUFFER;
	ibDesc.usage		= pgn::STATIC_DRAW;
	ibDesc.size			= indexBufferSize;

	pgn::Buffer* buf = rs->createBuffer(&ibDesc);
	geom->indexBuffer = buf;

	if(indices)
		buf->update(0, ibDesc.size, indices);
}

void GeometryHelper::destroyGeometry(Geometry* geom)
{
	int numAttribs = geom->vertexFormat->numAttribs;
	pgn::Buffer** vertexBuffers = geom->vertexBuffers;

	for(int i = 0; i < numAttribs; i++)
		vertexBuffers[i]->destroy();

	geom->indexBuffer->destroy();
}
