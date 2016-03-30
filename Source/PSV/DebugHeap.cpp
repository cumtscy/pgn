#define PGN_DLL_EXPORT
#include <PGN/Platform/DebugHeap.h>
#undef PGN_DLL_EXPORT

#include <PGN/Common/debug_new.h>
#define PGN_STATIC_IMPORT
#include <PGN/Platform/outputDebugInfo.h>
#undef PGN_STATIC_IMPORT
#include <sstream>
#include "DebugHeap.h"

std::map<void*, FileLine>* heapAllocMap = 0;

void pgn::beginDebugHeap()
{
	heapAllocMap = debug_new std::map<void*, FileLine>;
}

void pgn::endDebugHeap()
{
	for (auto it = heapAllocMap->begin(); it != heapAllocMap->end(); it++)
	{
		std::stringstream ss;
		ss << it->second.file << "(" << it->second.line << ")\n";
		outputDebugInfo((char*)ss.str().c_str());
	}
	delete heapAllocMap;
	heapAllocMap = 0;
}
