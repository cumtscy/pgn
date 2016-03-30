#include "ResourceView.h"

typedef ResourceView<pgn::DepthStencilView> BaseDepthStencilView;

class DepthStencilView : public BaseDepthStencilView
{
public:
	DepthStencilView(RenderBuffer* rb);
	DepthStencilView(Texture* tex);
};
