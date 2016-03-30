#include "CBufRange.h"

class Geometry;
class TextureInfo;

struct Batch
{
	Geometry*		geom;
	TextureInfo*	textureInfo;
	CBufRange		boneMatBuf;
	CBufRange		instanceCBlockBuf;
	int				instanceCount;
};
