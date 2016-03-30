#define PGN_DLL_EXPORT
#include <PGN/Utilities/Clock.h>
#undef PGN_DLL_EXPORT

#include <PGN/Common/debug_new.h>
#include <PGN/Platform/Time.h>

class Clock : public pgn::Clock
{
	long long cycleNumer, cycleDenom;

	long long baseNumer, baseDenom;

	long long numer, denom;

	long long start;

public:

	Clock(long long cycleNumer, long long cycleDenom)
	{
		pgn::getTimeUnit(&baseNumer, &baseDenom);
		setCycle(cycleNumer, cycleDenom);
		reset();
	}

	virtual void dispose()
	{
	}

	virtual void _free()
	{
		delete this;
	}

	virtual void destroy()
	{
		delete this;
	}

	virtual long long getTickCount()
	{
		long long current = pgn::getTime();

		if(start < 0) start = current;

		return (current - start) * numer / denom;
	}

	virtual void setCycle(long long numer, long long denom)
	{
		cycleNumer = numer;
		cycleDenom = denom;

		this->numer = baseNumer * cycleDenom;
		this->denom = baseDenom * cycleNumer;
	}

	virtual void getCycle(long long* numer, long long* denom)
	{
		*numer = cycleNumer;
		*denom = cycleDenom;
	}

	virtual void reset()
	{
		start = -1;
	}
};

pgn::Clock* pgn::Clock::create(long long cycleNumer, long long cycleDenom)
{
	return debug_new ::Clock(cycleNumer, cycleDenom);
}
