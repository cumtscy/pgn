#include <new>
#include <PGN/Utilities/Pool.h>
#include "PNA.h"
#include "PNAFactory.h"

PNAFactory::PNAFactory()
{
	pool = pgn::Pool::create(sizeof(PNA));
}

PNAFactory::~PNAFactory()
{
	pool->destroy();
}

pgn::Asset* PNAFactory::create()
{
	return new(pool->alloc()) PNA(this, boneAnimAllocator, rotKeyAllocator, posKeyAllocator);
}

void PNAFactory::recycle(pgn::Asset* asset)
{
	((PNA*)asset)->~PNA();
	pool->_free(asset);
}
