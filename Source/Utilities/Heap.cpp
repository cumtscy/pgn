#define PGN_DLL_EXPORT
#include <PGN/Utilities/Heap.h>
#undef PGN_DLL_EXPORT

#include <PGN/Common/debug_new.h>
#include <PGN/Platform/pageAlloc.h>
#include <vector>

namespace {

class Chunk
{
public:
	char* baseAddr;
	size_t chunkSize;
	size_t offset;

	Chunk(size_t size);
	~Chunk();
	void* alloc(size_t size);
	void clear();
};

}

Chunk::Chunk(size_t size)
{
	baseAddr = (char*)_pageAlloc(size);
	chunkSize = size;
	offset = 0;
}

Chunk::~Chunk()
{
	pgn::pageFree(baseAddr, chunkSize);
}

void* Chunk::alloc(size_t size)
{
	size_t _offset = offset;
	offset += size;

	if (offset > chunkSize)
		return 0;

	return baseAddr + _offset;
}

void Chunk::clear()
{
	offset = 0;
}

class Heap : public pgn::Heap
{
	size_t chunkSize;
	std::vector<Chunk*> chunks;
	int chunkIndex;
public:
	Heap(size_t chunkSize);
	virtual void dispose();
	virtual void _free();
	virtual void* alloc(size_t size);
	virtual void clear();
};

Heap::Heap(size_t chunkSize)
{
	this->chunkSize = chunkSize;
	chunks.push_back(debug_new Chunk(chunkSize));
	chunkIndex = 0;
}

void Heap::dispose()
{
	for (auto chunk : chunks)
		delete chunk;
}

pgn::Heap* pgn::Heap::create(size_t chunkSize)
{
	return debug_new ::Heap(chunkSize);
}

void Heap::_free()
{
	delete this;
}

void* Heap::alloc(size_t size)
{
	void* p = chunks[chunkIndex]->alloc(size);
	if(!p)
	{
		chunkIndex++;

		if (chunkIndex == chunks.size())
			chunks.push_back(debug_new Chunk(chunkSize));

		p = chunks[chunkIndex]->alloc(size);
	}
	return p;
}

void Heap::clear()
{
	for (auto chunk : chunks)
		chunk->clear();

	chunkIndex = 0;
}
