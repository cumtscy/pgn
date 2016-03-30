#include <cassert>
#include <gxm/init.h>
#include <gxm/memory.h>
#include <PGN/Common/debug_new.h>
#define PGN_DLL_IMPORT
#include <PGN/Platform/Graphics/GXM.h>
#undef PGN_DLL_IMPORT
#include <PGN/Platform/Graphics/RenderingContext.h>
#include <PGN/Utilities/ManagedHeap.h>
#include <stdlib.h>
#include <string.h>
#include "DepthStencilView.h"
#include "Program.h"
#include "RenderingSystem.h"
#include "RenderTargetView.h"
#include "SyncPoint.h"

void* patcherHostAlloc(void* userData, uint32_t size)
{
	return malloc(size);
}

void patcherHostFree(void* userData, void* mem)
{
	free(mem);
}

RenderingSystem::RenderingSystem(pgn::RenderingContext* rc)
{
	this->rc = rc;

	uint32_t patcherBufferSize = 64 * 1024;
	uint32_t patcherVertexUsseSize = 64 * 1024;
	uint32_t patcherFragmentUsseSize = 64 * 1024;

	void *patcherBuffer = pgn::graphicsAlloc(
		SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE,
		patcherBufferSize,
		4,
		SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE,
		&patcherBufferUid);

	uint32_t patcherVertexUsseOffset;
	void *patcherVertexUsse = pgn::vertexUsseAlloc(
		patcherVertexUsseSize,
		&patcherVertexUsseUid,
		&patcherVertexUsseOffset);

	uint32_t patcherFragmentUsseOffset;
	void *patcherFragmentUsse = pgn::fragmentUsseAlloc(
		patcherFragmentUsseSize,
		&patcherFragmentUsseUid,
		&patcherFragmentUsseOffset);

	SceGxmShaderPatcherParams patcherParams;
	memset(&patcherParams, 0, sizeof(SceGxmShaderPatcherParams));
	patcherParams.userData = NULL;
	patcherParams.hostAllocCallback = patcherHostAlloc;
	patcherParams.hostFreeCallback = patcherHostFree;
	patcherParams.bufferAllocCallback = NULL;
	patcherParams.bufferFreeCallback = NULL;
	patcherParams.bufferMem = patcherBuffer;
	patcherParams.bufferMemSize = patcherBufferSize;
	patcherParams.vertexUsseAllocCallback = NULL;
	patcherParams.vertexUsseFreeCallback = NULL;
	patcherParams.vertexUsseMem = patcherVertexUsse;
	patcherParams.vertexUsseMemSize = patcherVertexUsseSize;
	patcherParams.vertexUsseOffset = patcherVertexUsseOffset;
	patcherParams.fragmentUsseAllocCallback = NULL;
	patcherParams.fragmentUsseFreeCallback = NULL;
	patcherParams.fragmentUsseMem = patcherFragmentUsse;
	patcherParams.fragmentUsseMemSize = patcherFragmentUsseSize;
	patcherParams.fragmentUsseOffset = patcherFragmentUsseOffset;

	sceGxmShaderPatcherCreate(&patcherParams, &shaderPatcher);

	unsigned videoMemSize = pgn::getFreeVideoMemSize();
	void* videoMem = pgn::graphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RWDATA, videoMemSize, videoMemAlign, SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE, &videoMemUid);
	videoMemHeap = pgn::ManagedHeap::create(videoMem, videoMemSize, memcpy); // todo: 优化复制函数。

	volatile uint32_t* notificationRegion = sceGxmGetNotificationRegion();
	for (int i = 0; i < SCE_GXM_NOTIFICATION_COUNT; i++)
		notificationAddrFreeList.push_back(notificationRegion + i);
}

void RenderingSystem::dispose()
{
	for (auto entry : fpMap)
		sceGxmShaderPatcherReleaseFragmentProgram(shaderPatcher, entry.second.p);

	sceGxmShaderPatcherDestroy(shaderPatcher);

	pgn::graphicsFree(patcherBufferUid);
	pgn::vertexUsseFree(patcherVertexUsseUid);
	pgn::fragmentUsseFree(patcherFragmentUsseUid);

	videoMemHeap->destroy();
	pgn::graphicsFree(videoMemUid);
}

pgn::RenderingSystem* pgn::RenderingSystem::create(RenderingContext* rc)
{
	return debug_new ::RenderingSystem(rc);
}

void RenderingSystem::_free()
{
	delete this;
}

void RenderingSystem::beginFrame()
{
	rc->beginFrame();
}

void RenderingSystem::beginFrame(int numRenderTargets, pgn::RenderTargetView* renderTargets[], pgn::DepthStencilView* depthStencilView)
{
	RenderTargetView** rtvs = (RenderTargetView**)renderTargets;
	DepthStencilView* dsv = (DepthStencilView*)depthStencilView;

	RenderTargetAndMultisampleMode* renderTargetAndMultisampleMode = numRenderTargets ? &rtvs[0]->renderTargetAndMultisampleMode : &dsv->renderTargetAndMultisampleMode;
	SceGxmColorSurface* colorSurface = numRenderTargets ? &rtvs[0]->colorSurf : 0;
	SceGxmDepthStencilSurface* depthStencilSurface = dsv ? &dsv->depthStencilSurf : 0;

	sceGxmBeginScene(gxmContext, 0, renderTargetAndMultisampleMode->renderTarget, 0, 0, 0, colorSurface, depthStencilSurface);

	multisampleMode = renderTargetAndMultisampleMode->multisampleMode;
}

void RenderingSystem::clearDepthStencilView(pgn::DepthStencilView* depthStencilView, bool clearDepth, float depth, bool clearStencil, unsigned char stencil)
{
}

void RenderingSystem::clearRenderTargetView(pgn::RenderTargetView* renderTargetView, float r, float g, float b, float a)
{
}

void RenderingSystem::draw(int indicesPerInstance, int instanceCount)
{
	assert(sizeof(SceGxmBlendInfo) == sizeof(uint32_t));

	FPKey fpKey = { program, multisampleMode, blendInfo };
	FPPtr* ppFP = &fpMap[fpKey];

	if (!ppFP->p)
	{
		SceGxmFragmentProgram* fp;
		sceGxmShaderPatcherCreateFragmentProgram(shaderPatcher, program->fragmentProgramId, SCE_GXM_OUTPUT_REGISTER_FORMAT_DECLARED, multisampleMode, &blendInfo, 0, &fp);
		ppFP->p = fp;
	}

	sceGxmSetFragmentProgram(gxmContext, ppFP->p);

	sceGxmSetTwoSidedEnable(gxmContext, SCE_GXM_TWO_SIDED_ENABLED);

	static SceGxmCullMode cullModeMap[][3] =
	{
		// 逆时针为正面
		SCE_GXM_CULL_NONE,
		SCE_GXM_CULL_CW,
		SCE_GXM_CULL_CCW,

		// 顺时针为正面
		SCE_GXM_CULL_NONE,
		SCE_GXM_CULL_CCW,
		SCE_GXM_CULL_CW
	};

	sceGxmSetCullMode(gxmContext, cullModeMap[rasterizerStateDesc.frontFace][rasterizerStateDesc.cullFace]);

	static SceGxmDepthFunc depthFuncMap[] =
	{
		SCE_GXM_DEPTH_FUNC_NEVER, SCE_GXM_DEPTH_FUNC_LESS, SCE_GXM_DEPTH_FUNC_EQUAL, SCE_GXM_DEPTH_FUNC_LESS_EQUAL, SCE_GXM_DEPTH_FUNC_GREATER, SCE_GXM_DEPTH_FUNC_NOT_EQUAL, SCE_GXM_DEPTH_FUNC_GREATER_EQUAL, SCE_GXM_DEPTH_FUNC_ALWAYS
	};

	SceGxmDepthFunc depthFunc = depthFuncMap[depthStencilStateDesc.depthFunc];
	sceGxmSetFrontDepthFunc(gxmContext, depthFunc);
	sceGxmSetBackDepthFunc(gxmContext, depthFunc);

	SceGxmDepthWriteMode depthWrite = depthStencilStateDesc.depthWrite ? SCE_GXM_DEPTH_WRITE_ENABLED : SCE_GXM_DEPTH_WRITE_DISABLED;
	sceGxmSetFrontDepthWriteEnable(gxmContext, depthWrite);
	sceGxmSetBackDepthWriteEnable(gxmContext, depthWrite);

	static SceGxmStencilFunc stencilFunc[] =
	{
		SCE_GXM_STENCIL_FUNC_NEVER, SCE_GXM_STENCIL_FUNC_LESS, SCE_GXM_STENCIL_FUNC_EQUAL, SCE_GXM_STENCIL_FUNC_LESS_EQUAL, SCE_GXM_STENCIL_FUNC_GREATER, SCE_GXM_STENCIL_FUNC_NOT_EQUAL, SCE_GXM_STENCIL_FUNC_GREATER_EQUAL, SCE_GXM_STENCIL_FUNC_ALWAYS
	};

	static SceGxmStencilOp stencilOp[] =
	{
		SCE_GXM_STENCIL_OP_KEEP, SCE_GXM_STENCIL_OP_ZERO, SCE_GXM_STENCIL_OP_REPLACE, SCE_GXM_STENCIL_OP_INCR, SCE_GXM_STENCIL_OP_DECR, SCE_GXM_STENCIL_OP_INVERT, SCE_GXM_STENCIL_OP_INCR_WRAP, SCE_GXM_STENCIL_OP_DECR_WRAP
	};

	struct {
		SceGxmStencilFunc	stencilFunc;
		SceGxmStencilOp		stencilFail;
		SceGxmStencilOp		depthFail;
		SceGxmStencilOp		depthPass;
	} side[2]; // 0逆时针 1顺时针

	unsigned frontFace = rasterizerStateDesc.frontFace;
	unsigned backFace = frontFace ^ 1;

	side[frontFace].stencilFunc = stencilFunc[depthStencilStateDesc.frontFace.stencilFunc];
	side[frontFace].stencilFail = stencilOp[depthStencilStateDesc.frontFace.stencilFail];
	side[frontFace].depthFail = stencilOp[depthStencilStateDesc.frontFace.depthFail];
	side[frontFace].depthPass = stencilOp[depthStencilStateDesc.frontFace.depthPass];

	side[backFace].stencilFunc = stencilFunc[depthStencilStateDesc.backFace.stencilFunc];
	side[backFace].stencilFail = stencilOp[depthStencilStateDesc.backFace.stencilFail];
	side[backFace].depthFail = stencilOp[depthStencilStateDesc.backFace.depthFail];
	side[backFace].depthPass = stencilOp[depthStencilStateDesc.backFace.depthPass];

	sceGxmSetFrontStencilFunc(gxmContext, side[1].stencilFunc, side[1].stencilFail, side[1].depthFail, side[1].depthPass, depthStencilStateDesc.stencilReadMask, depthStencilStateDesc.stencilWriteMask);
	sceGxmSetBackStencilFunc(gxmContext, side[0].stencilFunc, side[0].stencilFail, side[0].depthFail, side[0].depthPass, depthStencilStateDesc.stencilReadMask, depthStencilStateDesc.stencilWriteMask);

	sceGxmSetFrontStencilRef(gxmContext, stencilRef);
	sceGxmSetBackStencilRef(gxmContext, stencilRef);

	int numVertexTextures = program->numVertexSamplers;

	for (int i = 0; i < numVertexTextures; i++)
		sceGxmSetVertexTexture(gxmContext, i, &textures[i]);

	int numFragmentTextures = program->getNumTextureVariables() - numVertexTextures;

	for (int i = 0; i < numFragmentTextures; i++)
		sceGxmSetFragmentTexture(gxmContext, i, &textures[numVertexTextures + i]);

	sceGxmDrawInstanced(gxmContext, primType, SCE_GXM_INDEX_FORMAT_U16, indexData, indicesPerInstance * instanceCount, indicesPerInstance);
}

void RenderingSystem::endFrame(pgn::SyncPoint* _sync)
{
	SyncPoint* sync = (SyncPoint*)_sync;

	sceGxmEndScene(gxmContext, 0, sync ? &sync->notification : 0);
}

void RenderingSystem::finish()
{
	sceGxmFinish(gxmContext);
}

void RenderingSystem::flush()
{
}

int RenderingSystem::getConstantBufferOffsetAlignment()
{
	return vecAlignInUniformBuffer;
}

const SceGxmPrimitiveType na = SCE_GXM_PRIMITIVE_POINTS;
const SceGxmPrimitiveType primTypes[] = { SCE_GXM_PRIMITIVE_POINTS, SCE_GXM_PRIMITIVE_LINES, na, SCE_GXM_PRIMITIVE_TRIANGLES, SCE_GXM_PRIMITIVE_TRIANGLE_STRIP };

void RenderingSystem::setPrimitiveType(pgn::PrimType primType)
{
	this->primType = primTypes[primType];
}

void RenderingSystem::setViewport(int left, int top, int width, int height, int fullHeight, float minDepth, float maxDepth)
{
}
