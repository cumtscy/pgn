#define PGN_DLL_EXPORT
#include <PGN/Platform/Thread/Semaphore.h>
#undef PGN_DLL_EXPORT

#include <Windows.h>
#include <PGN/Common/debug_new.h>

class Semaphore : public pgn::Semaphore
{
	HANDLE hSem;

public:
	Semaphore(int initCount)
	{
		hSem = CreateSemaphore(NULL, initCount, 10000, NULL);
	}

	virtual void dispose()
	{
		CloseHandle(hSem);
	}

	virtual void _free()
	{
		delete this;
	}

	virtual void acquire()
	{
		WaitForSingleObject(hSem, INFINITE);
	}

	virtual void release()
	{
		ReleaseSemaphore(hSem, 1, NULL);
	}
};

pgn::Semaphore* pgn::Semaphore::create(int initCount)
{
	return debug_new ::Semaphore(initCount);
}
