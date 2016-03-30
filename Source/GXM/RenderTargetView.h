#include "ResourceView.h"

typedef ResourceView<pgn::RenderTargetView> BaseRenderTargetView;

class RenderTargetView : public BaseRenderTargetView
{
public:
	RenderTargetView(RenderBuffer* rb);
	RenderTargetView(Texture* tex);
};
