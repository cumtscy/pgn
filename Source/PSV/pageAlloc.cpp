#define PGN_DLL_EXPORT
#include <PGN/Platform/pageAlloc.h>
#undef PGN_DLL_EXPORT

#include <kernel.h>
#include "DebugHeap.h"

unsigned pgn::getPageSize()
{
	return 4 * 1024;
}

void* pgn::pageAlloc(size_t size)
{
	SceUID uid = sceKernelAllocMemBlock("", SCE_KERNEL_MEMBLOCK_TYPE_USER_RW, size, 0);
	void* p;
	sceKernelGetMemBlockBase(uid, &p);
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

	SceUID uid = sceKernelFindMemBlockByAddr(p, size);
	sceKernelFreeMemBlock(uid);
}
