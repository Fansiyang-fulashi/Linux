#pragma once

#include"comm.hpp"
#include"mutex.hpp"

class Cond
{
public:
	Cond()
	{
		pthread_cond_init(&_cond,nullptr);
	}

	void Wait(Mutex& m)
	{
		pthread_cond_wait(&_cond,GetMutex(m));
	}

	pthread_mutex_t* GetMutex(Mutex& m)
	{
		return m.Get();
	}

	void Signal()
	{
		pthread_cond_signal(&_cond);
	}

	void Broadcast()
	{
		pthread_cond_broadcast(&_cond);
	}

	~Cond()
	{
		pthread_cond_destroy(&_cond);
	}
private:

	pthread_mutex_t* GetMutex(Mutex& m)
	{
		return m.Get();
	}

	pthread_cond_t _cond;
};