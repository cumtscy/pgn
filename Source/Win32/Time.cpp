#define PGN_DLL_EXPORT
#include <PGN/Platform/Time.h>
#undef PGN_DLL_EXPORT

#include <Windows.h>

void pgn::getTimeUnit(long long* numer, long long* denom)
{
	*numer = 1;
	QueryPerformanceFrequency((LARGE_INTEGER*)denom);
}

long long pgn::getTime()
{
	long long count;
	QueryPerformanceCounter((LARGE_INTEGER*)&count);
	return count;
}
