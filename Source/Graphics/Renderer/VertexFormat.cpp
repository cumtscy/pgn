#include <cassert>
#include <PGN/Common/debug_new.h>
#include <set>
#include <string>
#include "VertexFormat.h"

class VertexFormatLess
{
public:
	bool operator()(const VertexFormat& a, const VertexFormat& b)
	{
		if(a.numAttribs != b.numAttribs)
			return a.numAttribs < b.numAttribs;

		for(int i = 0; i < a.numAttribs; i++)
		{
			const char* name1 = a.vertexAttribDescs[i].name;
			const char* name2 = b.vertexAttribDescs[i].name;
			if(name1 != name2)
				return name1 < name2;
		}

		for (int i = 0; i < a.numAttribs; i++)
		{
			int stride1 = a.strides[i];
			int stride2 = b.strides[i];
			if (stride1 != stride2)
				return stride1 < stride2;
		}

		return false;
	}
};

static std::set<std::string>* attribNames;
static std::set<VertexFormat, VertexFormatLess>* formats;
static int refCount = 0;

void getVertexFormatTable()
{
	if (refCount++ == 0)
	{
		attribNames = debug_new std::set<std::string>;
		formats = debug_new std::set<VertexFormat, VertexFormatLess>;
	}
}

void releaseVertexFormatTable()
{
	if (--refCount == 0)
	{
		delete attribNames;
		delete formats;
	}
}

const VertexFormat* chooseVertexFormat(int numAttribs, pgn::VertexAttribDesc attribDescs[], int strides[])
{
	VertexFormat vf;

	vf.numAttribs = numAttribs;

	for(int i = 0; i < numAttribs; i++)
	{
		pgn::VertexAttribDesc* desc = &vf.vertexAttribDescs[i];
		*desc = attribDescs[i];
		auto ret = attribNames->insert(desc->name);
		desc->name = ret.first->c_str();
	}

	for (int i = 0; i < numAttribs; i++)
	{
		vf.strides[i] = strides[i];
	}

	auto ret = formats->insert(vf);
	return &*ret.first;
}
