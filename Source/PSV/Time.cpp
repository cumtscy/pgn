#define PGN_DLL_EXPORT
#include <PGN/Platform/Time.h>
#undef PGN_DLL_EXPORT

#include <kernel.h>

void pgn::getTimeUnit(long long* numer, long long* denom)
{
	*numer = 1;
	*denom = 1000000;
}

long long pgn::getTime()
{
	SceKernelSysClock count;
	sceKernelGetProcessTime(&count);
	return count.quad;
}
