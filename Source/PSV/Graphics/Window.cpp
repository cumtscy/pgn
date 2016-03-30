#include <display.h>
#include <gxm/init.h>
#include <gxm/memory.h>
#include <gxm/render_target.h>
#include <PGN/Common/debug_new.h>
#define PGN_STATIC_IMPORT
#include <PGN/Platform/Graphics/GXM.h>
#undef PGN_STATIC_IMPORT
#include <string.h>
#include "Window.h"

static int gxmRefCount = 0;

void displayCallback(const void *callbackData)
{
	sceDisplaySetFrameBuf((SceDisplayFrameBuf*)callbackData, SCE_DISPLAY_UPDATETIMING_NEXTVSYNC);
	sceDisplayWaitVblankStart();
}

#define align(x,a) ((x + ((a)-1)) & ~((a)-1))

Window::Window(int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, int samples, int w, int h)
{
	if (!gxmRefCount++)
	{
		SceGxmInitializeParams initializeParams;
		memset(&initializeParams, 0, sizeof(SceGxmInitializeParams));
		initializeParams.flags = 0;
		initializeParams.displayQueueMaxPendingCount = 2;
		initializeParams.displayQueueCallback = displayCallback;
		initializeParams.displayQueueCallbackDataSize = sizeof(SceDisplayFrameBuf);
		initializeParams.parameterBufferSize = SCE_GXM_DEFAULT_PARAMETER_BUFFER_SIZE;

		sceGxmInitialize(&initializeParams);
	}

	displayStrideInPixels = align(w, 64);
	int sizeDisplayBuf = 4 * displayStrideInPixels * h;

	SceGxmDepthStencilFormat depthStencilFormat;
	int bytesPerSample = 0;

	if (stencilBits)
	{
		if (depthBits)
		{
			depthStencilFormat = SCE_GXM_DEPTH_STENCIL_FORMAT_S8D24;
			bytesPerSample = 4;
		}
		else
		{
			// 注意sceGxmDepthStencilSurfaceInit后两个参数
			//depthStencilFormat = SCE_GXM_DEPTH_STENCIL_FORMAT_S8;
			//bytesPerSample = 1;
		}
	}
	else
	{
		if (depthBits > 16)
		{
			depthStencilFormat = SCE_GXM_DEPTH_STENCIL_FORMAT_S8D24;
			bytesPerSample = 4;
		}
		else if (depthBits > 0)
		{
			depthStencilFormat = SCE_GXM_DEPTH_STENCIL_FORMAT_D16;
			bytesPerSample = 2;
		}
	}

	SceGxmMultisampleMode multisampleMode;
	int depthStencilSurfaceStrideInSamples;
	int sizeDepthStencilBuf;

	if (samples == 4)
	{
		multisampleMode = SCE_GXM_MULTISAMPLE_4X;
		depthStencilSurfaceStrideInSamples = align(w * 2, SCE_GXM_TILE_SIZEX);
		sizeDepthStencilBuf = bytesPerSample * depthStencilSurfaceStrideInSamples * h * 2;
	}
	else if (samples == 2)
	{
		multisampleMode = SCE_GXM_MULTISAMPLE_2X;
		depthStencilSurfaceStrideInSamples = align(w, SCE_GXM_TILE_SIZEX);
		sizeDepthStencilBuf = bytesPerSample * depthStencilSurfaceStrideInSamples * h * 2;
	}
	else// if (samples == 1)
	{
		multisampleMode = SCE_GXM_MULTISAMPLE_NONE;
		depthStencilSurfaceStrideInSamples = align(w, SCE_GXM_TILE_SIZEX);
		sizeDepthStencilBuf = bytesPerSample * depthStencilSurfaceStrideInSamples * h;
	}

	int size = 0;

	for (int i = 0; i < displayBufferCount; i++)
	{
		size = align(size, 256);
		size += sizeDisplayBuf;
	}

	if (sizeDepthStencilBuf)
	{
		size = align(size, SCE_GXM_DEPTHSTENCIL_SURFACE_ALIGNMENT);
		size += sizeDepthStencilBuf;
	}

	union {
		char* mem;
		void* _mem;
	};

	_mem = pgn::graphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RWDATA, align(size, 256 * 1024), 0, SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE, &memBlockUID);

	int offset = 0;

	for (auto& displayBuffer : displayBufferChain)
	{
		offset = align(offset, 256);
		displayBuffer.displayBufferData = mem + offset;
		offset += sizeDisplayBuf;

		sceGxmColorSurfaceInit(
			&displayBuffer.displaySurface,
			SCE_GXM_COLOR_FORMAT_U8U8U8U8_ABGR,
			SCE_GXM_COLOR_SURFACE_LINEAR,
			multisampleMode == SCE_GXM_MULTISAMPLE_NONE ? SCE_GXM_COLOR_SURFACE_SCALE_NONE : SCE_GXM_COLOR_SURFACE_SCALE_MSAA_DOWNSCALE,
			SCE_GXM_OUTPUT_REGISTER_SIZE_32BIT,
			w,
			h,
			displayStrideInPixels,
			displayBuffer.displayBufferData);

		sceGxmSyncObjectCreate(&displayBuffer.displayBufferSync);
	}

	if (sizeDepthStencilBuf)
	{
		offset = align(size, SCE_GXM_DEPTHSTENCIL_SURFACE_ALIGNMENT);
		depthStencilBuffer.depthStencilBufferData = mem + offset;

		sceGxmDepthStencilSurfaceInit(
			&depthStencilBuffer.depthStencilSurface,
			depthStencilFormat,
			SCE_GXM_DEPTH_STENCIL_SURFACE_LINEAR,
			depthStencilSurfaceStrideInSamples,
			depthStencilBuffer.depthStencilBufferData,
			0);
	}

	SceGxmRenderTargetParams renderTargetParams;
	renderTargetParams.flags = 0;
	renderTargetParams.width = w;
	renderTargetParams.height = h;
	renderTargetParams.scenesPerFrame = 1;
	renderTargetParams.multisampleMode = multisampleMode;
	renderTargetParams.multisampleLocations = 0;
	renderTargetParams.driverMemBlock = SCE_UID_INVALID_UID;

	sceGxmCreateRenderTarget(&renderTargetParams, &renderTarget);

	backBufferIndex = 0;
	frontBufferIndex = 0;
	width = w;
	height = h;
}

void Window::dispose()
{
	void* mem;
	sceKernelGetMemBlockBase(memBlockUID, &mem);

	sceGxmUnmapMemory(mem);
	sceKernelFreeMemBlock(memBlockUID);

	for (auto& displayBuffer : displayBufferChain)
		sceGxmSyncObjectDestroy(displayBuffer.displayBufferSync);

	sceGxmDestroyRenderTarget(renderTarget);

	if (!--gxmRefCount)
		sceGxmTerminate();
}

pgn::Window* pgn::Window::create(int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, int samples, char title[], int x, int y, int clientWidth, int clientHeight, bool userResizable)
{
	return 0;
}

pgn::Window* pgn::Window::createFullScreen(int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, int samples, char title[], int w, int h)
{
	return debug_new ::Window(redBits, greenBits, blueBits, alphaBits, depthBits, stencilBits, samples, w, h);
}

pgn::Window* pgn::Window::create(int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, int samples, void* hWnd)
{
	return 0;
}

void Window::_free()
{
	delete this;
}

bool Window::processMessages()
{
	return true;
}

void* Window::getOSHandle()
{
	return 0;
}

void* Window::getDisplay()
{
	return this;
}

int Window::getClientWidth()
{
	return width;
}

int Window::getClientHeight()
{
	return height;
}

void Window::getClientSize(int* w, int* h)
{
	*w = width;
	*h = height;
}

void Window::present()
{
	SceDisplayFrameBuf framebuf;
	memset(&framebuf, 0x00, sizeof(SceDisplayFrameBuf));
	framebuf.size = sizeof(SceDisplayFrameBuf);
	framebuf.base = displayBufferChain[backBufferIndex].displayBufferData;
	framebuf.pitch = displayStrideInPixels;
	framebuf.pixelformat = SCE_GXM_COLOR_FORMAT_A8B8G8R8;
	framebuf.width = width;
	framebuf.height = height;

	sceGxmDisplayQueueAddEntry(displayBufferChain[frontBufferIndex].displayBufferSync, displayBufferChain[backBufferIndex].displayBufferSync, &framebuf);

	frontBufferIndex = backBufferIndex;
	backBufferIndex = (backBufferIndex + 1) % displayBufferCount;
}
