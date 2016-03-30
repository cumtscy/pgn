#define PGN_DLL_EXPORT
#include <PGN/Platform/Thread/Signal.h>
#undef PGN_DLL_EXPORT

#include <Windows.h>
#include <PGN/Common/debug_new.h>

class Signal : public pgn::Signal
{
	HANDLE hEvent;

public:
	Signal()
	{
		hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	virtual void dispose()
	{
		CloseHandle(hEvent);
	}

	virtual void _free()
	{
		delete this;
	}

	virtual void raise()
	{
		SetEvent(hEvent);
	}

	virtual void wait()
	{
		WaitForSingleObject(hEvent, INFINITE);
	}
};

pgn::Signal* pgn::Signal::create()
{
	return debug_new ::Signal;
}
