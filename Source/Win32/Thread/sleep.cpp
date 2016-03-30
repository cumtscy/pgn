#define PGN_DLL_EXPORT
#include <PGN/Platform/Thread/sleep.h>
#undef PGN_DLL_EXPORT

#include <Windows.h>

void pgn::sleep(unsigned milliSeconds)
{
	Sleep(milliSeconds);
}
