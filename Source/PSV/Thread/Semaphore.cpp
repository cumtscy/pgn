#define PGN_DLL_EXPORT
#include <PGN/Platform/Thread/Semaphore.h>
#undef PGN_DLL_EXPORT

#include <kernel.h>
#include <PGN/Common/debug_new.h>

class Semaphore : public pgn::Semaphore
{
	SceUID sem;

public:
	Semaphore(int initCount)
	{
		sem = sceKernelCreateSema("", SCE_KERNEL_SEMA_ATTR_TH_FIFO, initCount, 10000, 0);
	}

	virtual void dispose()
	{
		sceKernelDeleteSema(sem);
	}

	virtual void _free()
	{
		delete this;
	}

	virtual void acquire()
	{
		sceKernelWaitSema(sem, 1, 0);
	}

	virtual void release()
	{
		sceKernelSignalSema(sem, 1);
	}
};

pgn::Semaphore* pgn::Semaphore::create(int initCount)
{
	return debug_new::Semaphore(initCount);
}
