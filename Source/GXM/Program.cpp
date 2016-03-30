#include <PGN/Common/debug_new.h>
#include "Program.h"
#include "RenderingSystem.h"
#include "Shader.h"

Program::Program(Shader* vs, Shader* ps)
{
	vertexProgramId = vs->id;
	fragmentProgramId = ps->id;

	const SceGxmProgram* program;
	int paramCount;

	program = sceGxmShaderPatcherGetProgramFromId(vertexProgramId);
	paramCount = sceGxmProgramGetParameterCount(program);

	for (int i = 0; i < paramCount; i++)
	{
		const SceGxmProgramParameter* param = sceGxmProgramGetParameter(program, i);
		SceGxmParameterCategory cat = sceGxmProgramParameterGetCategory(param);

		if (cat == SCE_GXM_PARAMETER_CATEGORY_ATTRIBUTE)
			attribRegisterIndexMap[sceGxmProgramParameterGetName(param)] = sceGxmProgramParameterGetResourceIndex(param);

		else if (cat == SCE_GXM_PARAMETER_CATEGORY_SAMPLER)
			samplerNames.push_back(sceGxmProgramParameterGetName(param));
	}

	numVertexSamplers = samplerNames.size();

	program = sceGxmShaderPatcherGetProgramFromId(fragmentProgramId);
	paramCount = sceGxmProgramGetParameterCount(program);

	for (int i = 0; i < paramCount; i++)
	{
		const SceGxmProgramParameter* param = sceGxmProgramGetParameter(program, i);
		SceGxmParameterCategory cat = sceGxmProgramParameterGetCategory(param);

		if (cat == SCE_GXM_PARAMETER_CATEGORY_SAMPLER)
			samplerNames.push_back(sceGxmProgramParameterGetName(param));
	}
}

void Program::dispose()
{
}

pgn::Program* RenderingSystem::createProgram(pgn::Shader* vs, pgn::Shader* ps)
{
	return debug_new Program((Shader*)vs, (Shader*)ps);
}

pgn::Program* RenderingSystem::createProgram(pgn::Blob* programBlob)
{
	return 0;
}

void Program::_free()
{
	delete this;
}

int Program::getNumTextureVariables()
{
	return samplerNames.size();
}

const char* Program::getTextureVariableName(int i)
{
	return samplerNames[i].c_str();
}

pgn::Blob* RenderingSystem::serialize(pgn::Program* program)
{
	return 0;
}

void RenderingSystem::setProgram(pgn::Program* program)
{
	this->program = (Program*)program;
}
