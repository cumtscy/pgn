#include <mach/mach_time.h>
#include <PGN/Platform/Time.h>

void pgn::getTimeUnit(long long* numer, long long* denom)
{
    mach_timebase_info_data_t timeBase;
    mach_timebase_info(&timeBase);

    *numer = timeBase.numer;
    *denom =  timeBase.denom * 1e9;
}

long long pgn::getTime()
{
	return mach_absolute_time();
}
