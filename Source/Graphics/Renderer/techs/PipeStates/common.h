namespace pgn {
	struct DepthStencilStateDesc;
	struct OutputMergerStateDesc;
	struct RasterizerStateDesc;
	struct SamplerDesc;
}

extern char* shaderMacros[];
extern char* shaderMacroDefs[];
extern int numShaderMacros;

extern pgn::SamplerDesc* diffuseMapSamplerDesc;
extern pgn::SamplerDesc* pointSamplerDesc;
extern pgn::RasterizerStateDesc* commonRasterizerStateDesc;
extern pgn::RasterizerStateDesc* backFaceRasterizerStateDesc;
extern pgn::DepthStencilStateDesc* commonDepthStencilStateDesc;
extern pgn::DepthStencilStateDesc* depthStencilTestingDisabledStateDesc;
extern pgn::OutputMergerStateDesc* blendingDisabledStateDesc;
extern pgn::OutputMergerStateDesc* alphaBlendingStateDesc;
