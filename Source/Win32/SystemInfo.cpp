#define PGN_DLL_EXPORT
#include <PGN/Platform/SystemInfo.h>
#undef PGN_DLL_EXPORT

#include <Windows.h>

int pgn::getNumCPUs()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return info.dwNumberOfProcessors;
}
