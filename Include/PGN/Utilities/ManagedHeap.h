#pragma once
#include <stddef.h>
#include "../Common/DllInterface.h"
#include "../Common/Interface.h"
namespace pgn {

struct MemBlock
{
	char* p;
};

typedef void* (*CopyFunc)(void* dest, const void* src, size_t n);

class ManagedHeap : public Interface
{
public:
	static PGN_DLL_INTERFACE ManagedHeap* create(void* buf, size_t size, CopyFunc copy);

	virtual MemBlock* alloc(size_t size) = 0;
	virtual void _free(MemBlock* block) = 0;
	using Interface::_free;
	virtual void defragment(int maxNumBytes) = 0;
};

}
