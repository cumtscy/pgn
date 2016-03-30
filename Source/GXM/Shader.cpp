#include <PGN/Common/debug_new.h>
#include "RenderingSystem.h"
#include "Shader.h"

Shader::Shader(SceGxmShaderPatcher* shaderPatcher, pgn::ShaderDesc* desc)
{
	this->shaderPatcher = shaderPatcher;
	sceGxmShaderPatcherRegisterProgram(shaderPatcher, (SceGxmProgram*)desc->bin, &id);
}

void Shader::dispose()
{
	sceGxmShaderPatcherUnregisterProgram(shaderPatcher, id);
}

pgn::Shader* RenderingSystem::createShader(pgn::ShaderDesc* desc)
{
	return debug_new Shader(shaderPatcher, desc);
}

void Shader::_free()
{
	delete this;
}
