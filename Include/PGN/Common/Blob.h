#pragma once
#include "DllInterface.h"
#include "Interface.h"
namespace pgn {

class Blob : public Interface
{
public:
	static PGN_DLL_INTERFACE Blob* create(int size);

	virtual void* getBuffer() = 0;
	virtual int getSize() = 0;
};

}
