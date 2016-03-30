#include <PGN/Common/debug_new.h>
#include <PGN/Platform/DebugHeap.h>
#include <PGN/Platform/outputDebugInfo.h>
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
