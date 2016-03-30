#pragma once
#include <cassert>
#include <map>
#include <PGN/Common/debug_new.h>
namespace pgn {

template<class PoolType>
class PoolPtr
{
public:
	PoolType* p;

	PoolPtr()
	{
		p = 0;
	}
};

template<class PoolType>
class PoolSet
{
public:
	std::map<int, PoolPtr<PoolType>> poolMap;
	int refCount;

	PoolSet()
	{
		refCount = 1;
	}

	~PoolSet()
	{
		for (auto& entry : poolMap)
			entry.second.p->destroy();
	}

	void addRef()
	{
		refCount++;
	}

	void release()
	{
		if (--refCount == 0)
			delete this;
	}

	PoolType* getPool(int blockSize)
	{
		PoolPtr<PoolType>* ppPool = &poolMap[blockSize];
		PoolType* pool = ppPool->p;
		if (!pool)
		{
			pool = PoolType::create(blockSize);
			ppPool->p = pool;
		}
		return pool;
	}

	void shrink()
	{
		for (auto& entry : poolMap)
			entry.second.p->shrink();
	}
};

template<class T, class PoolType>
class PoolAllocator
{
public:
	typedef size_t		size_type;
	typedef T*			pointer;
	typedef const T*	const_pointer;
	typedef T			value_type;

	PoolSet<PoolType>* poolSet;
	PoolType* pool;

	PoolAllocator()
	{
		poolSet = debug_new PoolSet<PoolType>;
		pool = poolSet->getPool(sizeof(T));
	}

	PoolAllocator(const PoolAllocator& allocator)
	{
		poolSet = allocator.poolSet;
		poolSet->addRef();
		pool = poolSet->getPool(sizeof(T));
	}

	template<class U>
	PoolAllocator(const PoolAllocator<U, PoolType>& allocator)
	{
		poolSet = allocator.poolSet;
		poolSet->addRef();
		pool = poolSet->getPool(sizeof(T));
	}

	~PoolAllocator()
	{
		poolSet->release();
	}

	pointer allocate(size_t n)
	{
		assert(n == 1);
		return (pointer)pool->alloc();
	}

	void deallocate(pointer p, size_t n)
	{
		pool->_free(p);
	}

	template<class U>
	struct rebind
	{
		typedef PoolAllocator<U, PoolType> other;
	};

	void shrink()
	{
		poolSet->shrink();
	}
};

}
