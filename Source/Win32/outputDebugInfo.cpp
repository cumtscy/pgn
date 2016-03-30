#define PGN_DLL_EXPORT
#include <PGN/Platform/outputDebugInfo.h>
#undef PGN_DLL_EXPORT

#include <Windows.h>

void pgn::outputDebugInfo(char* str)
{
	OutputDebugStringA(str);
}
