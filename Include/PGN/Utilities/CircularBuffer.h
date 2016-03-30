#pragma once
#include <stddef.h>
#include "../Common/DllInterface.h"
#include "../Common/Interface.h"
namespace pgn {

class CircularBuffer : public Interface
{
public:
	static PGN_DLL_INTERFACE CircularBuffer* create(size_t bufSize);

	virtual void* alloc(size_t size) = 0;
	virtual void _free(size_t size) = 0;
	using Interface::_free;
};

}
