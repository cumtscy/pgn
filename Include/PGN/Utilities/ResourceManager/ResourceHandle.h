#pragma once
namespace pgn {

class ResourceHandle
{
public:
	virtual void* core() = 0;
};

}
