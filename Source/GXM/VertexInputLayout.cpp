#include <PGN/Common/debug_new.h>
#define PGN_DLL_IMPORT
#include <PGN/Platform/Graphics/GXM.h>
#undef PGN_DLL_IMPORT
#include "Program.h"
#include "RenderingSystem.h"
#include "VertexInputLayout.h"

struct AttribFormat
{
	SceGxmAttributeFormat format;
	unsigned componentCount;
};

const AttribFormat formats[] =
{
	SCE_GXM_ATTRIBUTE_FORMAT_F32, 1,	SCE_GXM_ATTRIBUTE_FORMAT_F32, 2,	SCE_GXM_ATTRIBUTE_FORMAT_F32, 3,	SCE_GXM_ATTRIBUTE_FORMAT_F32, 4,
	SCE_GXM_ATTRIBUTE_FORMAT_F16, 1,	SCE_GXM_ATTRIBUTE_FORMAT_F16, 2,	SCE_GXM_ATTRIBUTE_FORMAT_F16, 3,	SCE_GXM_ATTRIBUTE_FORMAT_F16, 4,
	SCE_GXM_ATTRIBUTE_FORMAT_S8,  1,	SCE_GXM_ATTRIBUTE_FORMAT_S8,  2,	SCE_GXM_ATTRIBUTE_FORMAT_S8,  3,	SCE_GXM_ATTRIBUTE_FORMAT_S8,  4,
	SCE_GXM_ATTRIBUTE_FORMAT_U8,  1,	SCE_GXM_ATTRIBUTE_FORMAT_U8,  2,	SCE_GXM_ATTRIBUTE_FORMAT_U8,  3,	SCE_GXM_ATTRIBUTE_FORMAT_U8,  4,
	SCE_GXM_ATTRIBUTE_FORMAT_S16, 1,	SCE_GXM_ATTRIBUTE_FORMAT_S16, 2,	SCE_GXM_ATTRIBUTE_FORMAT_S16, 3,	SCE_GXM_ATTRIBUTE_FORMAT_S16, 4,
	SCE_GXM_ATTRIBUTE_FORMAT_U16, 1,	SCE_GXM_ATTRIBUTE_FORMAT_U16, 2,	SCE_GXM_ATTRIBUTE_FORMAT_U16, 3,	SCE_GXM_ATTRIBUTE_FORMAT_U16, 4,
};

VertexInputLayout::VertexInputLayout(SceGxmShaderPatcher* shaderPatcher, Program* program, int numAttribs, const pgn::VertexAttribDesc attribDescs[], const int strides[])
{
	this->shaderPatcher = shaderPatcher;

	SceGxmVertexAttribute attribs[maxNumAttribs];
	SceGxmVertexStream streams[maxNumAttribs];
	int numActiveAttribs = 0;

	for(int i = 0; i < maxNumAttribs; i++)
		inputSlotToRegisterIndexMap[i] = -1;

	auto& attribRegisterIndexMap = program->attribRegisterIndexMap;

	for (int i = 0; i < numAttribs; i++)
	{
		const pgn::VertexAttribDesc* desc = &attribDescs[i];
		auto it = attribRegisterIndexMap.find(desc->name);

		if (it != attribRegisterIndexMap.end())
		{
			SceGxmVertexAttribute* attrib = &attribs[numActiveAttribs];
			SceGxmVertexStream* stream = &streams[numActiveAttribs];

			int registerIndex = it->second;

			attrib->streamIndex = registerIndex;
			attrib->offset = 0;
			attrib->format = formats[desc->format].format;
			attrib->componentCount = formats[desc->format].componentCount;
			attrib->regIndex = registerIndex;
			stream->stride = strides[i];
			stream->indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;

			inputSlotToRegisterIndexMap[i] = registerIndex;

			numActiveAttribs++;
		}
	}

	sceGxmShaderPatcherCreateVertexProgram(shaderPatcher, program->vertexProgramId, attribs, numActiveAttribs, streams, numActiveAttribs, &vertexProgram);
}

void VertexInputLayout::dispose()
{
	sceGxmShaderPatcherReleaseVertexProgram(shaderPatcher, vertexProgram);
}

pgn::VertexInputLayout* RenderingSystem::createVertexInputLayout(pgn::Program* program, int numAttribs, const pgn::VertexAttribDesc attribDescs[], const int strides[])
{
	return debug_new VertexInputLayout(shaderPatcher, (Program*)program, numAttribs, attribDescs, strides);
}

void VertexInputLayout::_free()
{
	delete this;
}

void VertexInputLayout::destroy()
{
	delete this;
}

void RenderingSystem::setVertexInputLayout(pgn::VertexInputLayout* layout)
{
	vertexInputLayout = (VertexInputLayout*)layout;

	sceGxmSetVertexProgram(gxmContext, vertexInputLayout->vertexProgram);
}
