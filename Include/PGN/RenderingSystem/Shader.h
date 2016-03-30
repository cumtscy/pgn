#pragma once
#include "../Common/Interface.h"
namespace pgn {

enum ShaderType
{
	VERTEX_SHADER,
	PIXEL_SHADER
};

struct ShaderDesc
{
	ShaderType	type;

#ifdef usePrecompiledShader
	void*		bin;
	int			size;
#else
	char*		src;
#endif
};

class Shader : public Interface
{
public:
};

}
