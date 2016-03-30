#include <gxm/shader_patcher.h>
#include <map>
#include <PGN/RenderingSystem/Program.h>
#include <string>
#include <vector>

class Shader;

class Program : public pgn::Program
{
public:
	SceGxmShaderPatcherId vertexProgramId;
	SceGxmShaderPatcherId fragmentProgramId;
	std::vector<std::string> samplerNames;
	int numVertexSamplers;
	std::map<std::string, int> attribRegisterIndexMap;

	Program(Shader* vs, Shader* ps);
	virtual void dispose();
	virtual void _free();
	virtual int getNumTextureVariables();
	virtual const char* getTextureVariableName(int i);
};
