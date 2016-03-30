#include <map>
#include <string>

struct FileLine
{
	std::string file;
	int line;
};

extern std::map<void*, FileLine>* heapAllocMap;
