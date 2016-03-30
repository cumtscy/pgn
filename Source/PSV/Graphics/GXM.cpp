#define PGN_DLL_EXPORT
#include <PGN/Platform/Graphics/GXM.h>
#undef PGN_DLL_EXPORT

#include <gxm/memory.h>
#include <kernel.h>
#include <libdbg.h>

#define ALIGN(x, a)					(((x) + ((a) - 1)) & ~((a) - 1))
#define	UNUSED(a)					(void)(a)

SceGxmContext* gxmContext;

unsigned pgn::getFreeVideoMemSize()
{
	SceKernelFreeMemorySizeInfo info;
	sceKernelGetFreeMemorySize(&info);

	return info.sizeCdram;
}

void* pgn::graphicsAlloc(SceKernelMemBlockType type, unsigned size, unsigned alignment, unsigned attribs, SceUID *uid)
{
	int err = SCE_OK;
	UNUSED(err);

	/*	Since we are using sceKernelAllocMemBlock directly, we cannot directly
	use the alignment parameter.  Instead, we must allocate the size to the
	minimum for this memblock type, and just assert that this will cover
	our desired alignment.

	Developers using their own heaps should be able to use the alignment
	parameter directly for more minimal padding.
	*/
	if (type == SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RWDATA) {
		// CDRAM memblocks must be 256KiB aligned
		SCE_DBG_ASSERT(alignment <= 256 * 1024);
		size = ALIGN(size, 256 * 1024);
	}
	else {
		// LPDDR memblocks must be 4KiB aligned
		SCE_DBG_ASSERT(alignment <= 4 * 1024);
		size = ALIGN(size, 4 * 1024);
	}
	UNUSED(alignment);

	// allocate some memory
	*uid = sceKernelAllocMemBlock("basic", type, size, 0);
	SCE_DBG_ASSERT(*uid >= SCE_OK);

	// grab the base address
	void *mem = 0;
	err = sceKernelGetMemBlockBase(*uid, &mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// map for the GPU
	err = sceGxmMapMemory(mem, size, attribs);
	SCE_DBG_ASSERT(err == SCE_OK);

	// done
	return mem;
}

void pgn::graphicsFree(SceUID uid)
{
	int err = SCE_OK;
	UNUSED(err);

	// grab the base address
	void *mem = 0;
	err = sceKernelGetMemBlockBase(uid, &mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// unmap memory
	err = sceGxmUnmapMemory(mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// free the memory block
	err = sceKernelFreeMemBlock(uid);
	SCE_DBG_ASSERT(err == SCE_OK);
}

void *pgn::vertexUsseAlloc(uint32_t size, SceUID *uid, uint32_t *usseOffset)
{
	int err = SCE_OK;
	UNUSED(err);

	// align to memblock alignment for LPDDR
	size = ALIGN(size, 4096);

	// allocate some memory
	*uid = sceKernelAllocMemBlock("basic", SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, size, 0);
	SCE_DBG_ASSERT(*uid >= SCE_OK);

	// grab the base address
	void *mem = 0;
	err = sceKernelGetMemBlockBase(*uid, &mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// map as vertex USSE code for the GPU
	err = sceGxmMapVertexUsseMemory(mem, size, usseOffset);
	SCE_DBG_ASSERT(err == SCE_OK);

	// done
	return mem;
}

void pgn::vertexUsseFree(SceUID uid)
{
	int err = SCE_OK;
	UNUSED(err);

	// grab the base address
	void *mem = 0;
	err = sceKernelGetMemBlockBase(uid, &mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// unmap memory
	err = sceGxmUnmapVertexUsseMemory(mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// free the memory block
	err = sceKernelFreeMemBlock(uid);
	SCE_DBG_ASSERT(err == SCE_OK);
}

void* pgn::fragmentUsseAlloc(unsigned size, SceUID *uid, unsigned *usseOffset)
{
	int err = SCE_OK;
	UNUSED(err);

	// align to memblock alignment for LPDDR
	size = ALIGN(size, 4096);

	// allocate some memory
	*uid = sceKernelAllocMemBlock("basic", SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, size, 0);
	SCE_DBG_ASSERT(*uid >= SCE_OK);

	// grab the base address
	void *mem = 0;
	err = sceKernelGetMemBlockBase(*uid, &mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// map as fragment USSE code for the GPU
	err = sceGxmMapFragmentUsseMemory(mem, size, usseOffset);
	SCE_DBG_ASSERT(err == SCE_OK);

	// done
	return mem;
}

void pgn::fragmentUsseFree(SceUID uid)
{
	int err = SCE_OK;
	UNUSED(err);

	// grab the base address
	void *mem = 0;
	err = sceKernelGetMemBlockBase(uid, &mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// unmap memory
	err = sceGxmUnmapFragmentUsseMemory(mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// free the memory block
	err = sceKernelFreeMemBlock(uid);
	SCE_DBG_ASSERT(err == SCE_OK);
}
