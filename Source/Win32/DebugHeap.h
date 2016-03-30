#include <map>
#include <string>

class FileLine
{
public:
	std::string file;
	int line;
};

extern std::map<void*, FileLine>* heapAllocMap;
