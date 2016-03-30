#pragma once
#include "../Common/Interface.h"
namespace pgn {

enum FrontFace
{
	CCW, CW
};

enum CullFace
{
	NONE,
	BACK_FACE,
	FRONT_FACE,
};

struct RasterizerStateDesc
{
	FrontFace	frontFace;
	CullFace	cullFace;
};

class RasterizerState : public Interface
{
public:
};

}
