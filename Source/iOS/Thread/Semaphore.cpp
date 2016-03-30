#include <mach/mach.h>
#include <PGN/Common/debug_new.h>
#include <PGN/Platform/Thread/Semaphore.h>

class Semaphore : public pgn::Semaphore
{
    semaphore_t sem;

public:
	Semaphore(int initCount)
	{
        semaphore_create(mach_task_self(), &sem, SYNC_POLICY_FIFO, initCount);
	}

	virtual void dispose()
	{
        semaphore_destroy(mach_task_self(), sem);
	}

	virtual void _free()
	{
		delete this;
	}

	virtual void acquire()
	{
        semaphore_wait(sem);
	}

	virtual void release()
	{
        semaphore_signal(sem);
	}
};

pgn::Semaphore* pgn::Semaphore::create(int initCount)
{
	return debug_new ::Semaphore(initCount);
}
