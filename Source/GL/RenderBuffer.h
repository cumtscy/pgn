#include <PGN/RenderingSystem/RenderBuffer.h>
#include "PixelFormat.h"

class RenderBuffer : public pgn::RenderBuffer
{
public:
	unsigned buf;

	RenderBuffer(pgn::RenderBufferDesc* desc);
	virtual void dispose();
	virtual void _free();
};
