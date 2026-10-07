#pragma once

#include"comm.hpp"

class Mutex
{
public:
	Mutex()
	{
		pthread_mutex_init(_pmt,nullptr);
	}

	~Mutex()
	{
		pthread_mutex_destroy(_pmt);
	}

	bool Lock()
	{
		int n=pthread_mutex_lock(_pmt);
	}

	bool Unlock()
	{
		int n=pthread_mutex_unlock(_pmt);
	}
	
private:
	pthread_mutex_t* _pmt;
};