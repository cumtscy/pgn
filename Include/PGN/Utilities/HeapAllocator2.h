#pragma once
#include <PGN/Common/debug_new.h>
#include "Heap.h"
namespace pgn {

struct HeapPtr
{
	Heap* p;
	int refCount;
};

template<class T>
class HeapAllocator2
{
public:
	typedef size_t		size_type;
	typedef T*			pointer;
	typedef const T*	const_pointer;
	typedef T			value_type;

	HeapPtr* ppHeap;

	HeapAllocator2()
	{
	}

	HeapAllocator2(Heap* heap)
	{
		ppHeap = debug_new HeapPtr;
		ppHeap->p = heap;
		ppHeap->refCount = 1;
	}

	HeapAllocator2(const HeapAllocator2& allocator)
	{
		ppHeap = allocator.ppHeap;
		ppHeap->refCount++;
	}

	template<class U>
	HeapAllocator2(const HeapAllocator2<U>& allocator)
	{
		ppHeap = allocator.ppHeap;
		ppHeap->refCount++;
	}

	~HeapAllocator2()
	{
		if(--ppHeap->refCount == 0)
			delete ppHeap;
	}

	void setPool(Heap* heap)
	{
		ppHeap->p = heap;
	}

	pointer allocate(size_t n)
	{
		return (pointer)ppHeap->p->alloc(sizeof(T)*n);
	}

	void deallocate(pointer p, size_t n)
	{
	}

	template<class U>
	struct rebind
	{
		typedef HeapAllocator2<U> other;
	};
};

}
