#include <PGN/Utilities/ResourceManager/ResourceHandle.h>
#include <string>

namespace pgn {
	class Asset;
}

class ResourceHandle : public pgn::ResourceHandle
{
public:
	std::string name;
	void* _core;
	pgn::Asset* asset;
	int refCount;

	ResourceHandle();
	virtual void* core();
};
