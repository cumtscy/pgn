#include <gxm/shader_patcher.h>
#include <PGN/RenderingSystem/Shader.h>

class Shader : public pgn::Shader
{
public:
	SceGxmShaderPatcher* shaderPatcher;
	SceGxmShaderPatcherId id;

	Shader(SceGxmShaderPatcher* shaderPatcher, pgn::ShaderDesc* desc);
	virtual void dispose();
	virtual void _free();
};
