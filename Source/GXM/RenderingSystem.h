#include <map>
#define PGN_DLL_EXPORT
#include <PGN/RenderingSystem/DepthStencilState.h>
#include <PGN/RenderingSystem/RenderingSystem.h>
#include <PGN/RenderingSystem/RasterizerState.h>
#undef PGN_DLL_EXPORT

#include <gxm/shader_patcher.h>
#include <kernel/sysmem/memblock.h>
#include <vector>

namespace pgn {
	class ManagedHeap;
}

class Program;
class SceGxmShaderPatcher;
class VertexInputLayout;

class RenderingSystem : public pgn::RenderingSystem
{
public:
	pgn::RenderingContext* rc;
	SceUID patcherBufferUid;
	SceUID patcherVertexUsseUid;
	SceUID patcherFragmentUsseUid;
	SceGxmShaderPatcher* shaderPatcher;
	SceUID videoMemUid;
	pgn::ManagedHeap* videoMemHeap;
	std::vector<volatile uint32_t*> notificationAddrFreeList;

	VertexInputLayout* vertexInputLayout;

	// 以下成员变量按draw函数里用到的顺序定义。
	Program* program;
	SceGxmMultisampleMode multisampleMode;
	SceGxmBlendInfo blendInfo;

	struct FPKey
	{
		Program* program;
		SceGxmMultisampleMode multisampleMode;
		union {
			SceGxmBlendInfo blendInfo;
			uint32_t _blendInfo;
		};
	};

	class FPKeyLess
	{
	public:
		bool operator()(const FPKey& a, const FPKey& b) const
		{
			return a.program < b.program
				|| a.multisampleMode < b.multisampleMode
				|| a._blendInfo < b._blendInfo;
		}
	};

	class FPPtr
	{
	public:
		SceGxmFragmentProgram* p;

		FPPtr()
		{
			p = 0;
		}
	};

	std::map<FPKey, FPPtr, FPKeyLess> fpMap;

	pgn::RasterizerStateDesc rasterizerStateDesc;
	pgn::DepthStencilStateDesc depthStencilStateDesc;
	uint8_t stencilRef;

	SceGxmTexture textures[pgn::maxNumTextures];

	SceGxmPrimitiveType primType;
	void* indexData;

	RenderingSystem(pgn::RenderingContext* rc);
	virtual void dispose();
	virtual void _free();
	virtual void beginFrame();
	virtual void beginFrame(int numRenderTargets, pgn::RenderTargetView* renderTargets[], pgn::DepthStencilView* depthStencilView);
	virtual bool checkSyncPoint(pgn::SyncPoint* sync);
	virtual pgn::Buffer*			createBuffer(pgn::BufferDesc* desc);
	virtual pgn::DepthStencilState*	createDepthStencilState(pgn::DepthStencilStateDesc* desc);
	virtual pgn::DepthStencilView*	createDepthStencilView(pgn::RenderBuffer* rb);
	virtual pgn::DepthStencilView*	createDepthStencilView(pgn::Texture* tex);
	virtual pgn::OutputMergerState*	createOutputMergerState(pgn::OutputMergerStateDesc* desc);
	virtual pgn::Program*			createProgram(pgn::Shader* vs, pgn::Shader* ps);
	virtual pgn::Program*			createProgram(pgn::Blob* programBlob);
	virtual pgn::RasterizerState*	createRasterizerState(pgn::RasterizerStateDesc* desc);
	virtual pgn::RenderBuffer*		createRenderBuffer(pgn::RenderBufferDesc* desc);
	virtual pgn::RenderTargetView*	createRenderTargetView(pgn::RenderBuffer* rb);
	virtual pgn::RenderTargetView*	createRenderTargetView(pgn::Texture* tex);
	virtual pgn::Sampler*			createSampler(pgn::SamplerDesc* desc);
	virtual pgn::Shader*			createShader(pgn::ShaderDesc* desc);
	virtual pgn::SyncPoint*			createSyncPoint();
	virtual pgn::Texture*			createTexture(pgn::TextureDesc* desc, void* levels[]);
	virtual pgn::VertexInputLayout*	createVertexInputLayout(pgn::Program* program, int numAttribs, const pgn::VertexAttribDesc attribDescs[], const int strides[]);
	virtual void clearDepthStencilView(pgn::DepthStencilView* depthStencilView, bool clearDepth, float depth, bool clearStencil, unsigned char stencil);
	virtual void clearRenderTargetView(pgn::RenderTargetView* renderTargetView, float r, float g, float b, float a);
	virtual void draw(int indicesPerInstance, int instanceCount);
	virtual void endFrame(pgn::SyncPoint* sync);
	virtual void finish();
	virtual void flush();
	virtual int getConstantBufferOffsetAlignment();
	virtual pgn::Blob* serialize(pgn::Program* program);
	virtual void setConstantBuffers(int startSlot, int numBuffers, pgn::Buffer* buffers[], int offsets[], int sizes[]);
	virtual void setDepthStencilState(pgn::DepthStencilState* state, unsigned stencilRef);
	virtual void setIndexBuffer(pgn::Buffer* buf, int offset);
	virtual void setOutputMergerState(pgn::OutputMergerState* state);
	virtual void setPrimitiveType(pgn::PrimType primType);
	virtual void setProgram(pgn::Program* program);
	virtual void setRasterizerState(pgn::RasterizerState* state);
	virtual void setSamplers(int startSlot, int numSamplers, pgn::Sampler* samplers[]);
	virtual void setTextures(int startSlot, int numTextures, pgn::Texture* textures[]);
	virtual void setVertexBuffers(int startSlot, int numBuffers, pgn::Buffer* buffers[], int offsets[]);
	virtual void setVertexInputLayout(pgn::VertexInputLayout* layout);
	virtual void setViewport(int left, int top, int width, int height, int fullHeight, float minDepth, float maxDepth);
};

const int vecAlignInUniformBuffer = 16; // 其实应该是8。详见Shader Compiler User's Guide -> Profiles -> Semantics -> Uniform Buffers。
const int videoMemAlign = vecAlignInUniformBuffer;
