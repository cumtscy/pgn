#define PGN_DLL_EXPORT
#include <PGN/Platform/Graphics/RenderingContext.h>
#undef PGN_DLL_EXPORT

#include <gxm/context.h>
#include <gxm/memory.h>
#include <PGN/Common/debug_new.h>
#include <stdlib.h>
#include <string.h>

#define PGN_STATIC_IMPORT
#include <PGN/Platform/Graphics/GXM.h>
#undef PGN_STATIC_IMPORT

#include "Window.h"

class RenderingContext : public pgn::RenderingContext
{
	void* hostMem;
	SceUID vdmRingBufferUid;
	SceUID vertexRingBufferUid;
	SceUID fragmentRingBufferUid;
	SceUID fragmentUsseRingBufferUid;

	SceGxmContext* context;

	Window* wnd;
public:
	RenderingContext(void* hDC, int numThreads);
	virtual void dispose();
	virtual void _free();
	virtual void beginDraw(void* wnd, int thread);
	virtual void endDraw();
	virtual void beginFrame();
};

RenderingContext::RenderingContext(void* hDC, int numThreads)
{
	hostMem = malloc(SCE_GXM_MINIMUM_CONTEXT_HOST_MEM_SIZE);

	void* vdmRingBuffer = pgn::graphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, SCE_GXM_DEFAULT_VDM_RING_BUFFER_SIZE, 4, SCE_GXM_MEMORY_ATTRIB_READ, &vdmRingBufferUid);
	void* vertexRingBuffer = pgn::graphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, SCE_GXM_DEFAULT_VERTEX_RING_BUFFER_SIZE, 4, SCE_GXM_MEMORY_ATTRIB_READ, &vertexRingBufferUid);
	void* fragmentRingBuffer = pgn::graphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, SCE_GXM_DEFAULT_FRAGMENT_RING_BUFFER_SIZE, 4, SCE_GXM_MEMORY_ATTRIB_READ, &fragmentRingBufferUid);

	unsigned fragmentUsseRingBufferOffset;
	void* fragmentUsseRingBuffer = pgn::fragmentUsseAlloc(SCE_GXM_DEFAULT_FRAGMENT_USSE_RING_BUFFER_SIZE, &fragmentUsseRingBufferUid, &fragmentUsseRingBufferOffset);

	SceGxmContextParams contextParams;
	memset(&contextParams, 0, sizeof(SceGxmContextParams));
	contextParams.hostMem = hostMem;
	contextParams.hostMemSize = SCE_GXM_MINIMUM_CONTEXT_HOST_MEM_SIZE;
	contextParams.vdmRingBufferMem = vdmRingBuffer;
	contextParams.vdmRingBufferMemSize = SCE_GXM_DEFAULT_VDM_RING_BUFFER_SIZE;
	contextParams.vertexRingBufferMem = vertexRingBuffer;
	contextParams.vertexRingBufferMemSize = SCE_GXM_DEFAULT_VERTEX_RING_BUFFER_SIZE;
	contextParams.fragmentRingBufferMem = fragmentRingBuffer;
	contextParams.fragmentRingBufferMemSize = SCE_GXM_DEFAULT_FRAGMENT_RING_BUFFER_SIZE;
	contextParams.fragmentUsseRingBufferMem = fragmentUsseRingBuffer;
	contextParams.fragmentUsseRingBufferMemSize = SCE_GXM_DEFAULT_FRAGMENT_USSE_RING_BUFFER_SIZE;
	contextParams.fragmentUsseRingBufferOffset = fragmentUsseRingBufferOffset;

	sceGxmCreateContext(&contextParams, &context);
}

void RenderingContext::dispose()
{
	sceGxmDestroyContext(context);
	pgn::fragmentUsseFree(fragmentUsseRingBufferUid);
	pgn::graphicsFree(fragmentRingBufferUid);
	pgn::graphicsFree(vertexRingBufferUid);
	pgn::graphicsFree(vdmRingBufferUid);
	free(hostMem);
}

pgn::RenderingContext* pgn::RenderingContext::create(void* hDC, int numThreads)
{
	return debug_new ::RenderingContext(hDC, numThreads);
}

void RenderingContext::_free()
{
	delete this;
}

void RenderingContext::beginDraw(void* wnd, int thread)
{
	this->wnd = (Window*)wnd;
	gxmContext = context;
}

void RenderingContext::endDraw()
{
}

void RenderingContext::beginFrame()
{
	DisplayBuffer* displayBuffer = &wnd->displayBufferChain[wnd->backBufferIndex];
	sceGxmBeginScene(context, 0, wnd->renderTarget, 0, 0, displayBuffer->displayBufferSync, &displayBuffer->displaySurface, &wnd->depthStencilBuffer.depthStencilSurface);
}
