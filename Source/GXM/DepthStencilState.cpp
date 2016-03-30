#include <PGN/Common/debug_new.h>
#include <PGN/RenderingSystem/DepthStencilState.h>
#include "RenderingSystem.h"

class DepthStencilState : public pgn::DepthStencilState
{
public:
	pgn::DepthStencilStateDesc desc;

	DepthStencilState(pgn::DepthStencilStateDesc* desc);
	virtual void dispose();
	virtual void _free();
	virtual void destroy();
};

DepthStencilState::DepthStencilState(pgn::DepthStencilStateDesc* desc)
{
	this->desc = *desc;
}

void DepthStencilState::dispose()
{
}

pgn::DepthStencilState* RenderingSystem::createDepthStencilState(pgn::DepthStencilStateDesc* desc)
{
	return debug_new DepthStencilState(desc);
}

void DepthStencilState::_free()
{
	delete this;
}

void DepthStencilState::destroy()
{
	delete this;
}

void RenderingSystem::setDepthStencilState(pgn::DepthStencilState* state, unsigned stencilRef)
{
	pgn::DepthStencilStateDesc* desc = &((DepthStencilState*)state)->desc;

	depthStencilStateDesc = *desc;
	this->stencilRef = stencilRef;
}
