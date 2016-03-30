#define PGN_DLL_EXPORT
#include <PGN/Platform/Thread/Mutex.h>
#undef PGN_DLL_EXPORT

#include <kernel.h>
#include <PGN/Common/debug_new.h>

class Mutex : public pgn::Mutex
{
	SceUID mutex;

public:
	Mutex()
	{
		mutex = sceKernelCreateMutex("", SCE_KERNEL_MUTEX_ATTR_TH_FIFO, 0, 0);
	}

	virtual void dispose()
	{
		sceKernelDeleteMutex(mutex);
	}

	virtual void _free()
	{
		delete this;
	}

	virtual void get()
	{
		sceKernelLockMutex(mutex, 1, 0);
	}

	virtual bool tryGet()
	{
		return sceKernelTryLockMutex(mutex, 1) == SCE_OK;
	}

	virtual void release()
	{
		sceKernelUnlockMutex(mutex, 0);
	}
};

pgn::Mutex* pgn::Mutex::create()
{
	return debug_new::Mutex;
}
