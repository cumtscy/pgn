#define PGN_DLL_EXPORT
#include <PGN/Platform/Graphics/Window.h>
#undef PGN_DLL_EXPORT

#include <gxm/surface.h>
#include <scebase.h>

struct DisplayBuffer
{
	void *displayBufferData;
	SceGxmColorSurface displaySurface;
	SceGxmSyncObject *displayBufferSync;
};

struct DepthStencilBuffer
{
	void* depthStencilBufferData;
	SceGxmDepthStencilSurface depthStencilSurface;
};

class Window : public pgn::Window
{
public:
	SceUID memBlockUID;

	static const int displayBufferCount = 3;
	DisplayBuffer displayBufferChain[displayBufferCount];
	DepthStencilBuffer depthStencilBuffer;

	SceGxmRenderTarget *renderTarget;

	int backBufferIndex;
	int frontBufferIndex;

	int displayStrideInPixels;
	int width;
	int height;

	Window(int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, int samples, int w, int h);
	virtual void dispose();
	virtual void _free();
	virtual bool processMessages();
	virtual void* getOSHandle();
	virtual void* getDisplay();
	virtual int getClientWidth();
	virtual int getClientHeight();
	virtual void getClientSize(int* w, int* h);
	virtual void present();
};
