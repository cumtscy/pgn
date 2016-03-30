#pragma once
#include "../../Common/DllInterface.h"
#include "../../Common/Interface.h"
namespace pgn {

class Signal : public Interface
{
public:
	static PGN_DLL_INTERFACE Signal* create();

	virtual void raise() = 0;
	virtual void wait() = 0;
};

}
