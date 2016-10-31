#include <map>
#include <PGN/Utilities/ResourceManager/ResourceHandle.h>
#include <string>

namespace pgn {
	class Asset;
}

class ResourceHandle : public pgn::ResourceHandle
{
public:
	std::map<std::string, ResourceHandle>::iterator it;
	void* _core;
	pgn::Asset* asset;
	int refCount;

	ResourceHandle();
	virtual void* core();
};
