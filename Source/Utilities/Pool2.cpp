#define PGN_DLL_EXPORT
#include <PGN/Utilities/Pool2.h>
#undef PGN_DLL_EXPORT

#include <cassert>
#include <list>
#include <map>
#include <PGN/Common/debug_new.h>
#include <PGN/Platform/pageAlloc.h>
#include <vector>

namespace {

class Chunk
{
public:
	static const size_t chunkSize = 1 * 1024 * 1024;
	char* baseAddr;
	size_t capacity;
	std::vector<void*> freeList;

	Chunk(size_t blockSize);
	~Chunk();
	void* alloc();
	void _free(void* p);
	bool full();
	bool empty();
};

}

Chunk::Chunk(size_t blockSize)
{
	baseAddr = (char*)_pageAlloc(chunkSize);
	capacity = chunkSize / blockSize;
	freeList.resize(capacity);
	for (unsigned i = 0; i < capacity; i++)
		freeList[i] = baseAddr + blockSize * i;
}

Chunk::~Chunk()
{
	assert(empty());
	pgn::pageFree(baseAddr, chunkSize);
}

void* Chunk::alloc()
{
	void* p = freeList.back();
	freeList.pop_back();
	return p;
}

void Chunk::_free(void* p)
{
	freeList.push_back(p);
}

bool Chunk::full()
{
	return freeList.empty();
}

bool Chunk::empty()
{
	return freeList.size() == capacity;
}

class Range
{
public:
	void* begin;
	void* end;

	Range(char* p)
	{
		begin = p;
		end = p + Chunk::chunkSize - 1;
	}

	Range(void* p)
	{
		begin = p;
		end = p;
	}
};

class RangeLess
{
public:
	bool operator()(const Range& a, const Range& b) const
	{
		return a.end < b.begin;
	}
};

class Pool2 : public pgn::Pool2
{
	size_t blockSize;
	std::map<Range, Chunk*, RangeLess> inverseMap;
	std::list<Chunk*> availableChunks;
public:
	Pool2(size_t blockSize);
	virtual void dispose();
	virtual void _free();
	virtual void* alloc();
	virtual void _free(void* p);
	virtual void shrink();
};

Pool2::Pool2(size_t blockSize)
{
	this->blockSize = blockSize;
	Chunk* chunk = debug_new Chunk(blockSize);
	inverseMap[chunk->baseAddr] = chunk;
	availableChunks.push_back(chunk);
}

void Pool2::dispose()
{
	for(auto chunk : availableChunks)
		delete chunk;
}

pgn::Pool2* pgn::Pool2::create(size_t blockSize)
{
	return debug_new ::Pool2(blockSize);
}

void Pool2::_free()
{
	delete this;
}

void* Pool2::alloc()
{
	if(availableChunks.empty())
	{
		Chunk* chunk = debug_new Chunk(blockSize);
		inverseMap[chunk->baseAddr] = chunk;
		availableChunks.push_back(chunk);
	}

	Chunk* chunk = availableChunks.front();
	void* p = chunk->alloc();

	if(chunk->full())
		availableChunks.pop_front();

	return p;
}

void Pool2::_free(void* p)
{
	Chunk* chunk = inverseMap[p];

	if(chunk->full())
		availableChunks.push_front(chunk);

	chunk->_free(p);
}

void Pool2::shrink()
{
	auto it = availableChunks.begin();
	while(it != availableChunks.end())
	{
		Chunk* chunk = *it;
		if(chunk->empty() && availableChunks.size() > 1)
		{
			delete chunk;
			it = availableChunks.erase(it);
		}
		else
		{
			it++;
		}
	}
}
