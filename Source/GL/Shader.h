#include <map>
#include <PGN/RenderingSystem/Shader.h>

class Shader : public pgn::Shader
{
public:
	unsigned type;
	unsigned shader;
	std::map<std::string, int> uniformBlockBindingPointMap;
	std::map<std::string, int> samplerVariableBindingPointMap;

	Shader(pgn::ShaderDesc* desc);
	virtual void dispose();
	virtual void _free();
};
