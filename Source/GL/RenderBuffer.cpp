#include <PGN/Common/debug_new.h>
#include "RenderBuffer.h"
#include "RenderingSystem.h"

RenderBuffer::RenderBuffer(pgn::RenderBufferDesc* desc)
{
	const PixelFormat* format = &formats[desc->format];

	glGenRenderbuffers(1, &buf);
	glBindRenderbuffer(GL_RENDERBUFFER, buf);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, desc->samples, format->internalFormat, desc->width, desc->height);
}

void RenderBuffer::dispose()
{
	glDeleteRenderbuffers(1, &buf);
}

pgn::RenderBuffer* RenderingSystem::createRenderBuffer(pgn::RenderBufferDesc* desc)
{
	return debug_new RenderBuffer(desc);
}

void RenderBuffer::_free()
{
	delete this;
}
