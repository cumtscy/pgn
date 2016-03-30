#define PGN_DLL_EXPORT
#include <PGN/Platform/outputDebugInfo.h>
#undef PGN_DLL_EXPORT

#include <stdio.h>

void pgn::outputDebugInfo(char* str)
{
	printf(str);
}
