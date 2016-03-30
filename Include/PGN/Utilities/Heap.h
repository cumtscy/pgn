#pragma once
#include <stddef.h>
#include "../Common/DllInterface.h"
#include "../Common/Interface.h"
namespace pgn {

class Heap : public Interface
{
public:
	static PGN_DLL_INTERFACE Heap* create(size_t chunkSize = 64 * 1024);

	virtual void* alloc(size_t size) = 0;
	virtual void clear() = 0;
};

}
