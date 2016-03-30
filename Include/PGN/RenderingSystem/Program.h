#pragma once
#include "../Common/Interface.h"
namespace pgn {

class Program : public Interface
{
public:
	virtual int getNumTextureVariables() = 0;
	virtual const char* getTextureVariableName(int i) = 0; // 相同的名字总是返回相同的指针，即使对于不同的Program。
};

}
