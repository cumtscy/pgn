#pragma once
#include "../../Common/DllInterface.h"
#include "../../Common/Interface.h"
namespace pgn {

class Mutex : public Interface
{
public:
	static PGN_DLL_INTERFACE Mutex* create();

	virtual void get() = 0;
	virtual bool tryGet() = 0;
	virtual void release() = 0;
};

}
