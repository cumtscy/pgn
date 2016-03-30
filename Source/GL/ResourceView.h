#include <PGN/RenderingSystem/DepthStencilView.h>
#include <PGN/RenderingSystem/RenderTargetView.h>
#include "RenderBuffer.h"
#include "Texture.h"

enum ResourceType
{
	RENDER_BUFFER,
	TEXTURE
};

template<class I>
class ResourceView : public I
{
public:
	ResourceType type;
	union {
		unsigned rb;
		unsigned tex;
	};

	ResourceView(RenderBuffer* rb)
	{
		type = RENDER_BUFFER;
		this->rb = rb->buf;
	}
	ResourceView(Texture* tex)
	{
		type = TEXTURE;
		this->tex = tex->tex;
	}
	virtual void dispose(){}
	virtual void _free(){delete this;}
	virtual void destroy(){delete this;}
};

typedef ResourceView<pgn::DepthStencilView> DepthStencilView;
typedef ResourceView<pgn::RenderTargetView> RenderTargetView;
