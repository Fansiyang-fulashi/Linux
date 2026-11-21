#pragma once

#include"comm.hpp"

class Sem
{
public:
	Sem(unsigned int value)
	{
		sem_init(&_sem,0,value);
	}

	~Sem()
	{
		sem_destroy(&_sem);
	}

	void V()
	{
		sem_post(&_sem);
	}

	void P()
	{
		sem_wait(&_sem);
	}

private:
	sem_t _sem;
};