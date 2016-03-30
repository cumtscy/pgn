#pragma once
#include <stddef.h>
#include "../Common/DllInterface.h"
#include "../Common/Interface.h"
namespace pgn {

class Pool2 : public Interface
{
public:
	static PGN_DLL_INTERFACE Pool2* create(size_t blockSize);

	virtual void* alloc() = 0;
	virtual void _free(void* p) = 0;
	using Interface::_free;
	virtual void shrink() = 0;
};

}
