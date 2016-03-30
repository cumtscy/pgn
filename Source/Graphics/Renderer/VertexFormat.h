#include <PGN/RenderingSystem/VertexInputLayout.h>

struct VertexFormat
{
public:
	static const int maxNumAttribs = 16;
	int numAttribs;
	pgn::VertexAttribDesc vertexAttribDescs[maxNumAttribs];
	int strides[maxNumAttribs];
};

void getVertexFormatTable();
void releaseVertexFormatTable();
const VertexFormat* chooseVertexFormat(int numAttribs, pgn::VertexAttribDesc attribDescs[], int strides[]);
