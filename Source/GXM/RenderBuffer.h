#include <gxm/structs.h>
#include <PGN/RenderingSystem/RenderBuffer.h>

namespace pgn {
	class ManagedHeap;
	class MemBlock;
}

class RenderBuffer : public pgn::RenderBuffer
{
public:
	pgn::PixelFormat format;
	int width;
	int height;
	int samples;

	union {
		int strideInSamples;
		int strideInPixels;
	};

	pgn::ManagedHeap* heap;
	pgn::MemBlock* memBlock;

	RenderBuffer(pgn::RenderBufferDesc* desc, pgn::ManagedHeap* heap);
	virtual void dispose();
	virtual void _free();
};
