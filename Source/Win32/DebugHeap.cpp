#define PGN_DLL_EXPORT
#include <PGN/Platform/DebugHeap.h>
#undef PGN_DLL_EXPORT

#include <PGN/Common/debug_new.h>
#define PGN_STATIC_IMPORT
#include <PGN/Platform/outputDebugInfo.h>
#undef PGN_STATIC_IMPORT
#include <sstream>
#include <Windows.h>
#include "DebugHeap.h"

std::map<void*, FileLine>* heapAllocMap = 0;
static _CrtMemState heapState;

void pgn::beginDebugHeap()
{
	heapAllocMap = debug_new std::map<void*, FileLine>;
	_CrtMemCheckpoint(&heapState);
}

void pgn::endDebugHeap()
{
	_CrtMemDumpAllObjectsSince(&heapState);
	for (auto& entry : *heapAllocMap)
	{
		std::stringstream ss;
		ss << entry.second.file << "(" << entry.second.line << ")\n";
		outputDebugInfo((char*)ss.str().c_str());
	}
	delete heapAllocMap;
	heapAllocMap = 0;
}
