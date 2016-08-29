#pragma once
#include "../../Common/Interface.h"
namespace pgn {

class SkeletonTemplate : public Interface
{
public:
	virtual int getNumBones() = 0;
};

}
