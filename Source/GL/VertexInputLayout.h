#include <PGN/RenderingSystem/VertexInputLayout.h>

class Program;

struct Attribute
{
	bool		enabled;
	int			size;
	unsigned	type;
	int			divisor;
};

class VertexInputLayout : public pgn::VertexInputLayout
{
public:
	static const int maxNumAttribs = 16;
	Attribute attribs[maxNumAttribs];
	int inputSlotToLocationMap[maxNumAttribs];
	int strides[maxNumAttribs];

	VertexInputLayout(Program* program, int numAttribs, const pgn::VertexAttribDesc attribDescs[], const int strides[]);
	virtual void dispose();
	virtual void _free();
	virtual void destroy();
};
