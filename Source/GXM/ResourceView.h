#pragma once
#include <gxm/render_target.h>
#include <PGN/RenderingSystem/DepthStencilView.h>
#include <PGN/RenderingSystem/RenderTargetView.h>
#include "RenderBuffer.h"
#include "Texture.h"

struct RenderTargetAndMultisampleMode
{
	SceGxmRenderTarget *renderTarget;
	SceGxmMultisampleMode multisampleMode;
};

template<class I>
class ResourceView : public I
{
public:

	RenderTargetAndMultisampleMode renderTargetAndMultisampleMode;

	union {
		SceGxmColorSurface colorSurf;
		SceGxmDepthStencilSurface depthStencilSurf;
	};

	ResourceView(RenderBuffer* rb)
	{
		if (rb->samples == 4)
			renderTargetAndMultisampleMode.multisampleMode = SCE_GXM_MULTISAMPLE_4X;
		else if (rb->samples == 2)
			renderTargetAndMultisampleMode.multisampleMode = SCE_GXM_MULTISAMPLE_2X;
		else// if (rb->samples == 1)
			renderTargetAndMultisampleMode.multisampleMode = SCE_GXM_MULTISAMPLE_NONE;

		SceGxmRenderTargetParams renderTargetParams;
		renderTargetParams.flags = 0;
		renderTargetParams.width = rb->width;
		renderTargetParams.height = rb->height;
		renderTargetParams.scenesPerFrame = 1;
		renderTargetParams.multisampleMode = renderTargetAndMultisampleMode.multisampleMode;
		renderTargetParams.multisampleLocations = 0;
		renderTargetParams.driverMemBlock = SCE_UID_INVALID_UID;

		sceGxmCreateRenderTarget(&renderTargetParams, &renderTargetAndMultisampleMode.renderTarget);
	}

	ResourceView(Texture* tex)
	{
		renderTargetAndMultisampleMode.multisampleMode = SCE_GXM_MULTISAMPLE_NONE;

		SceGxmRenderTargetParams renderTargetParams;
		renderTargetParams.flags = 0;
		renderTargetParams.width = tex->desc.width;
		renderTargetParams.height = tex->desc.height;
		renderTargetParams.scenesPerFrame = 1;
		renderTargetParams.multisampleMode = renderTargetAndMultisampleMode.multisampleMode;
		renderTargetParams.multisampleLocations = 0;
		renderTargetParams.driverMemBlock = SCE_UID_INVALID_UID;

		sceGxmCreateRenderTarget(&renderTargetParams, &renderTargetAndMultisampleMode.renderTarget);
	}

	virtual void dispose()
	{
		sceGxmDestroyRenderTarget(renderTargetAndMultisampleMode.renderTarget);
	}

	virtual void _free()
	{
		delete this;
	}
};
