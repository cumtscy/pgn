#define PGN_DLL_EXPORT
#include <PGN/Platform/Thread/Mutex.h>
#undef PGN_DLL_EXPORT

#include <Windows.h>
#include <PGN/Common/debug_new.h>

class Mutex : public pgn::Mutex
{
	CRITICAL_SECTION cs;

public:
	Mutex()
	{
		InitializeCriticalSection(&cs);
	}

	virtual void dispose()
	{
		DeleteCriticalSection(&cs);
	}

	virtual void _free()
	{
		delete this;
	}

	virtual void get()
	{
		EnterCriticalSection(&cs);
	}

	virtual bool tryGet()
	{
		return !!TryEnterCriticalSection(&cs);
	}

	virtual void release()
	{
		LeaveCriticalSection(&cs);
	}
};

pgn::Mutex* pgn::Mutex::create()
{
	return debug_new ::Mutex;
}
