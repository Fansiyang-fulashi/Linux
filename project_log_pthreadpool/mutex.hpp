#pragma once

#include"comm.hpp"

class Mutex
{
public:
	Mutex()
	{
		pthread_mutex_init(&_pmt,nullptr);
	}

	~Mutex()
	{
		pthread_mutex_destroy(&_pmt);
	}

	void Lock()
	{
		int n=pthread_mutex_lock(&_pmt);
	}

	void Unlock()
	{
		int n=pthread_mutex_unlock(&_pmt);
	}
	
	pthread_mutex_t* Get()
	{
		return &_pmt;
	}
private:
	pthread_mutex_t _pmt;
};

class LockGuard
{
public:
	LockGuard(Mutex& mutex)
	:_mutex(mutex)
	{
		_mutex.Lock();
	}

	~LockGuard()
	{
		_mutex.Unlock();
	}
private:
	Mutex& _mutex;
};