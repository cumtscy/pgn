#define PGN_DLL_EXPORT
#include <PGN/Platform/Thread/sleep.h>
#undef PGN_DLL_EXPORT

#include <kernel.h>

void pgn::sleep(unsigned milliSeconds)
{
	sceKernelDelayThread(milliSeconds * 1000);
}
