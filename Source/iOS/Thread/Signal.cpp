#include <PGN/Common/debug_new.h>
#include <PGN/Platform/Thread/Signal.h>
#include <pthread.h>

class Signal : public pgn::Signal
{
    bool signaled;
    pthread_mutex_t mutex;
	pthread_cond_t cond;

public:
	Signal()
	{
        signaled = false;
        pthread_mutex_init(&mutex, NULL);
		pthread_cond_init(&cond, NULL);
	}

	virtual void dispose()
	{
        pthread_mutex_destroy(&mutex);
		pthread_cond_destroy(&cond);
	}

	virtual void _free()
	{
		delete this;
	}

	virtual void raise()
	{
		pthread_mutex_lock(&mutex);
        signaled = true;
        pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cond);
	}

	virtual void wait()
	{
		pthread_mutex_lock(&mutex);
        if(!signaled)
            pthread_cond_wait(&cond, &mutex);
        signaled = false;
        pthread_mutex_unlock(&mutex);
	}
};

pgn::Signal* pgn::Signal::create()
{
	return debug_new ::Signal;
}
