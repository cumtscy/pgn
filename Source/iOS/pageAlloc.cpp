#include <mach/mach.h>
#include <PGN/Platform/pageAlloc.h>
#include "DebugHeap.h"

unsigned pgn::getPageSize()
{
    vm_size_t pageSize;
    host_page_size(mach_host_self(), &pageSize);
    return (unsigned)pageSize;
}

void* pgn::pageAlloc(size_t size)
{
    void* p;
	vm_allocate(mach_task_self(), (vm_address_t*)&p, size, VM_FLAGS_ANYWHERE);
    return p;
}

void* pgn::pageAlloc(size_t size, const char file[], int line)
{
	void* p = pageAlloc(size);

	if (heapAllocMap)
	{
		FileLine fileLine;
		fileLine.file = file;
		fileLine.line = line;
		(*heapAllocMap)[p] = fileLine;
	}

	return p;
}

void pgn::pageFree(void* p, size_t size)
{
	if (heapAllocMap)
		heapAllocMap->erase(p);

	vm_deallocate(mach_host_self(), (vm_address_t)p, size);
}
