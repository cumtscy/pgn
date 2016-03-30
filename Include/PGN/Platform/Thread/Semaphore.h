#pragma once
#include "../../Common/DllInterface.h"
#include "../../Common/Interface.h"
namespace pgn {

class Semaphore : public Interface
{
public:
	static PGN_DLL_INTERFACE Semaphore* create(int initCount);

	virtual void acquire() = 0;
	virtual void release() = 0;
};

}
