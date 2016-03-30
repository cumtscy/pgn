#pragma once
#include <PGN/Common/debug_new.h>
#include "Heap.h"
namespace pgn {

template<class T>
class HeapAllocator
{
public:
	typedef size_t		size_type;
	typedef T*			pointer;
	typedef const T*	const_pointer;
	typedef T			value_type;

	Heap* heap;

	HeapAllocator(Heap* heap)
	{
		this->heap = heap;
	}

	HeapAllocator(const HeapAllocator& allocator)
	{
		heap = allocator.heap;
	}

	template<class U>
	HeapAllocator(const HeapAllocator<U>& allocator)
	{
		heap = allocator.heap;
	}

	pointer allocate(size_t n)
	{
		return (pointer)heap->alloc(sizeof(T)*n);
	}

	void deallocate(pointer p, size_t n)
	{
	}

	template<class U>
	struct rebind
	{
		typedef HeapAllocator<U> other;
	};
};

}
