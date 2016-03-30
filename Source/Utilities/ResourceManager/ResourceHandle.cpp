#include <PGN/Utilities/ResourceManager/Asset.h>
#include "ResourceHandle.h"

ResourceHandle::ResourceHandle()
{
	_core = 0;
	refCount = 0;
}

void* ResourceHandle::core()
{
	if (!_core)
		_core = asset->core();

	return _core;
}
