#include <cassert>
#include <gxm/memory.h>
#include <PGN/Common/debug_new.h>
#define PGN_DLL_IMPORT
#include <PGN/Platform/Graphics/GXM.h>
#undef PGN_DLL_IMPORT
#include <PGN/RenderingSystem/Buffer.h>
#include <PGN/Utilities/ManagedHeap.h>
#include <string.h>
#include "RenderingSystem.h"
#include "VertexInputLayout.h"

class Buffer : public pgn::Buffer
{
public:
	virtual void _free();
};

class StaticDrawBuffer : public Buffer
{
public:
	pgn::ManagedHeap* heap;
	pgn::MemBlock* memBlock;

	StaticDrawBuffer(int size, pgn::ManagedHeap* heap);
	virtual void dispose();
	virtual void update(int offset, int size, void* data);
	virtual void* map(int offset, int size);
	virtual void unmap();
};

class DynamicDrawBuffer : public Buffer
{
public:
	SceUID uid;
	char* p;

	DynamicDrawBuffer(int size);
	virtual void dispose();
	virtual void update(int offset, int size, void* data);
	virtual void* map(int offset, int size);
	virtual void unmap();
};

StaticDrawBuffer::StaticDrawBuffer(int size, pgn::ManagedHeap* heap)
{
	this->heap = heap;

	assert(!(size % videoMemAlign));
	memBlock = heap->alloc(size);
}

void StaticDrawBuffer::dispose()
{
	heap->_free(memBlock);
}

void StaticDrawBuffer::update(int offset, int size, void* data)
{
	memcpy(memBlock->p + offset, data, size); // todo: 优化。
}

void* StaticDrawBuffer::map(int offset, int size)
{
	return memBlock->p + offset;
}

void StaticDrawBuffer::unmap()
{
}

DynamicDrawBuffer::DynamicDrawBuffer(int size)
{
	p = (char*)pgn::graphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE, size, 4, SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE, &uid);
}

void DynamicDrawBuffer::dispose()
{
	pgn::graphicsFree(uid);
}

void DynamicDrawBuffer::update(int offset, int size, void* data)
{
	memcpy(p + offset, data, size); // todo: 优化。
}

void* DynamicDrawBuffer::map(int offset, int size)
{
	return p + offset;
}

void DynamicDrawBuffer::unmap()
{
}

pgn::Buffer* RenderingSystem::createBuffer(pgn::BufferDesc* desc)
{
	if (desc->usage == pgn::BufferUsage::STATIC_DRAW)
		return debug_new StaticDrawBuffer(desc->size, videoMemHeap);
	else// if (desc->usage == pgn::BufferUsage::DYNAMIC_DRAW)
		return debug_new DynamicDrawBuffer(desc->size);
}

void Buffer::_free()
{
	delete this;
}

void RenderingSystem::setVertexBuffers(int startSlot, int numBuffers, pgn::Buffer* buffers[], int offsets[])
{
	for(int i = 0, slot = startSlot; i < numBuffers; i++, slot++)
	{
		int registerIndex = vertexInputLayout->inputSlotToRegisterIndexMap[slot];

		if (registerIndex >= 0)
			sceGxmSetVertexStream(gxmContext, registerIndex, buffers[i]->map(offsets[i], 0));
	}
}

void RenderingSystem::setIndexBuffer(pgn::Buffer* buf, int offset)
{
	indexData = buf->map(offset, 0);
}

void RenderingSystem::setConstantBuffers(int startSlot, int numBuffers, pgn::Buffer* buffers[], int offsets[], int sizes[])
{
	for(int i = 0; i < numBuffers; i++)
	{
		int bufferIndex = startSlot + i;
		sceGxmSetVertexUniformBuffer(gxmContext, bufferIndex, buffers[i]->map(offsets[i], 0));
	}
}
