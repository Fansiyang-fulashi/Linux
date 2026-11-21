#pragma once

#include"comm.hpp"
#include"sem.hpp"
#include"mutex.hpp"

#define gcap 5

template<class T>
class RingQueue
{
public:
	RingQueue(int cap=gcap)
	:_rq(cap)
	,_cap(cap)
	,_blank_sem(cap)
	,_p_step(0)
	,_data_sem(0)
	,_c_step(0)
	{}

	void Push(const T& data)
	{
		_blank_sem.P();
		{
			LockGuard lockguard(_pmutex);
			_rq[_p_step] = data;
			_p_step++;
			_p_step %= _cap;
			_data_sem.V();
		}
	}

	void Pop(T* out)
	{
		_data_sem.P();
		{
			LockGuard lockguard(_cmutex);
			*out = _rq[_c_step];
			_c_step++;
			_c_step %= _cap;
			_blank_sem.V();
		}
	}

	~RingQueue()
	{}

private:
	std::vector<T> _rq;
	int _cap;
	Sem _blank_sem;
	int _p_step;
	Sem _data_sem;
	int _c_step;

	Mutex _pmutex;
	Mutex _cmutex;
};