#include <PGN/RenderingSystem/VertexInputLayout.h>

class Program;
struct SceGxmShaderPatcher;
struct SceGxmVertexProgram;

class VertexInputLayout : public pgn::VertexInputLayout
{
public:
	SceGxmShaderPatcher* shaderPatcher;
	SceGxmVertexProgram* vertexProgram;
	static const int maxNumAttribs = 16;
	int inputSlotToRegisterIndexMap[maxNumAttribs];

	VertexInputLayout(SceGxmShaderPatcher* shaderPatcher, Program* program, int numAttribs, const pgn::VertexAttribDesc attribDescs[], const int strides[]);
	virtual void dispose();
	virtual void _free();
	virtual void destroy();
};
