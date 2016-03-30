#pragma once
#include "../Common/DllInterface.h"
#include "../Common/Interface.h"
namespace pgn {

class Animation;
class FileStream;

class AnimationFactory : public Interface
{
public:
	static PGN_DLL_INTERFACE AnimationFactory* create(FileStream* f);

	virtual Animation* createAnimation() = 0;
};

}
