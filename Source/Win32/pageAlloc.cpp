#define PGN_DLL_EXPORT
#include <PGN/Platform/pageAlloc.h>
#undef PGN_DLL_EXPORT

#include <Windows.h>
#include "DebugHeap.h"

unsigned pgn::getPageSize()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return info.dwAllocationGranularity;
}

void* pgn::pageAlloc(size_t size)
{
	return VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
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

	VirtualFree(p, 0, MEM_RELEASE);
}
