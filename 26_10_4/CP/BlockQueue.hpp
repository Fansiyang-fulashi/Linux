#pragma once

#include"comm.hpp"
#include"mutex.hpp"
#include"cond.hpp"

#define NUM_MAX 5

template<class T>
class BlockQueue
{
public:
	BlockQueue()
	:_cap(NUM_MAX)
	,_csleep_num(0)
	,_psleep_num(0)
	{
	}
	
	~BlockQueue()
	{
	}

	void Push(const T& in)
	{
		{
			LockGuard lockguard(_mutex);
			while (IsFull())
			{
				_psleep_num++;
				_full_cond.Wait(_mutex);
				_psleep_num--;
			}
			_q.push(in);
			if (_csleep_num > 0)
			{
				_empty_cond.Signal();
			}
		}
	}

	T Pop()
	{
		{
			LockGuard lockguard(_mutex);
			while (IsEmpty())
			{
				_csleep_num++;
				_empty_cond.Wait(_mutex);
				_csleep_num--;
			}
			T data = _q.front();
			_q.pop();
			if (_psleep_num > 0)
			{
				_full_cond.Signal();
			}
			return data;
		}
	}
private:
	bool IsFull()
	{
		return _q.size()>=_cap;
	}

	bool IsEmpty()
	{
		return _q.size()==0;
	}

	std::queue<T> _q;
	int _cap;
	Mutex _mutex;
	Cond _full_cond;
	Cond _empty_cond;
	int _csleep_num;
	int _psleep_num;
};