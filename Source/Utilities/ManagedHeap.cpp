#define PGN_DLL_EXPORT
#include <PGN/Utilities/ManagedHeap.h>
#undef PGN_DLL_EXPORT

#include <cassert>
#include <PGN/Common/debug_new.h>
#define PGN_STATIC_IMPORT
#include <PGN/Utilities/Pool.h>
#undef PGN_STATIC_IMPORT
#include <string.h>

struct MemBlock : public pgn::MemBlock
{
	size_t size;
	bool free;
	MemBlock* prev;
	MemBlock* next;
};

class ManagedHeap : public pgn::ManagedHeap
{
	pgn::Pool* blockPool;
	MemBlock blockList;
	MemBlock* firstFreeBlock;
	pgn::CopyFunc copy;
public:
	ManagedHeap(void* buf, size_t size, pgn::CopyFunc copy);
	inline MemBlock* merge(MemBlock* a, MemBlock* b);
	virtual void dispose();
	virtual void _free();
	virtual pgn::MemBlock* alloc(size_t size);
	virtual void _free(pgn::MemBlock* block);
	virtual void defragment(int maxNumBytes);
};

ManagedHeap::ManagedHeap(void* buf, size_t size, pgn::CopyFunc copy)
{
	blockPool = pgn::Pool::create(sizeof(MemBlock));

	MemBlock* freeBlock = (MemBlock*)blockPool->alloc();
	freeBlock->p = (char*)buf;
	freeBlock->size = size;
	freeBlock->free = true;

	freeBlock->prev = &blockList;
	freeBlock->next = &blockList;
	blockList.prev = freeBlock;
	blockList.next = freeBlock;

	firstFreeBlock = freeBlock;

	this->copy = copy;
}

void ManagedHeap::dispose()
{
	blockPool->destroy();
}

pgn::ManagedHeap* pgn::ManagedHeap::create(void* buf, size_t size, CopyFunc copy)
{
	return debug_new ::ManagedHeap(buf, size, copy);
}

void ManagedHeap::_free()
{
	delete this;
}

pgn::MemBlock* ManagedHeap::alloc(size_t size)
{
	MemBlock* block = firstFreeBlock;
	while (!block->free || block->size < size)
	{
		block = block->next;
		assert(block != &blockList);
	}

	if (block->size == size)
	{
		block->free = false;

		MemBlock* freeBlock = block->next;

		while (freeBlock != &blockList && !freeBlock->free)
			freeBlock = freeBlock->next;

		if (freeBlock != &blockList)
			firstFreeBlock = freeBlock;

		return block;
	}

	MemBlock* newBlock = (MemBlock*)blockPool->alloc();
	newBlock->p = block->p;
	newBlock->size = size;
	newBlock->free = false;

	block->p += size;
	block->size -= size;

	newBlock->prev = block->prev;
	newBlock->next = block;
	block->prev->next = newBlock;
	block->prev = newBlock;

	return newBlock;
}

MemBlock* ManagedHeap::merge(MemBlock* a, MemBlock* b)
{
	assert(a->next == b);

	a->size += b->size;

	a->next = b->next;
	b->next->prev = a;

	blockPool->_free(b);

	return a;
}

void ManagedHeap::_free(pgn::MemBlock* _block)
{
	MemBlock* block = (MemBlock*)_block;

	MemBlock* prev = block->prev;
	MemBlock* next = block->next;

	if (prev != &blockList && prev->free)
		block = merge(prev, block);

	if (next != &blockList && next->free)
		merge(block, next);

	block->free = true;

	if (block->p < firstFreeBlock->p)
		firstFreeBlock = block;
}

void ManagedHeap::defragment(int maxNumBytes)
{
	MemBlock* firstFreeBlock = this->firstFreeBlock;
	firstFreeBlock->prev->next = firstFreeBlock->next;
	firstFreeBlock->next->prev = firstFreeBlock->prev;

	MemBlock* block = firstFreeBlock->next;

	while (maxNumBytes > 0 && block != &blockList)
	{
		char* dest = firstFreeBlock->p;
		char* src = block->p;

		size_t n = 0;

		while (n < maxNumBytes && block != &blockList && !block->free)
		{
			n += block->size;

			block->p = firstFreeBlock->p;
			firstFreeBlock->p += n;

			block = block->next;
		}

		copy(dest, src, n);
		maxNumBytes -= (int)n;
	}

	firstFreeBlock->prev = block->prev;
	firstFreeBlock->next = block;
	block->prev->next = firstFreeBlock;
	block->prev = firstFreeBlock;
}
