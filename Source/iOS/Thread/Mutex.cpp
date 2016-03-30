#include <PGN/Common/debug_new.h>
#include <PGN/Platform/Thread/Mutex.h>
#include <pthread.h>

class Mutex : public pgn::Mutex
{
	pthread_mutex_t mutex;

public:
	Mutex()
	{
		pthread_mutex_init(&mutex, NULL);
	}

	virtual void dispose()
	{
		pthread_mutex_destroy(&mutex);
	}

	virtual void _free()
	{
		delete this;
	}

	virtual void get()
	{
		pthread_mutex_lock(&mutex);
	}

	virtual bool tryGet()
	{
		return pthread_mutex_trylock(&mutex);
	}

	virtual void release()
	{
		pthread_mutex_unlock(&mutex);
	}
};

pgn::Mutex* pgn::Mutex::create()
{
	return debug_new ::Mutex;
}
