#pragma once

#include"thread.hpp"
#include"log.hpp"
#include<vector>
#include<queue>
#include<unistd.h>
#include"cond.hpp"

template<class T>
class ThreadPool
{
	static const int num=5;

public:

	static ThreadPool<T>* GetInstance()
	{
		if(_th==nullptr)
		{
			LockGuard lockguard(_lock);
			if (_th == nullptr)
			{
				_th = new ThreadPool<T>();
				_th->Start();
			}
		}
		return _th;
	}

	void Stop()
	{
		if(!_isrunning) return;
		_isrunning=false;
		WakeUpAll();
	}

	void HandlerTask()
	{
		while(true)
		{
			T task;
			{
				LockGuard lockguard(_mutex);
				while (_taskq.empty()&&_isrunning)
				{
					_sleepnum++;
					_cond.Wait(_mutex);
					_sleepnum--;
				}
				if(_taskq.empty()&&!_isrunning)
				{
					LOG(LogLevel::FATAL)<<"线程退出";
					break;
				}
				task=_taskq.front();
				_taskq.pop();
			}
			task();
		}
	}

	void Join()
	{
		for(auto& t:_threads)
		{
			LOG(LogLevel::DEBUG)<<"join";
			t.Join();
		}
	}


	void Push(const T& in)
	{
		if(_isrunning)
		{
			LockGuard lockguard(_mutex);
			_taskq.push(in);
			if(_sleepnum==_threads.size())
			WakeUpOne();
		}
	}

	~ThreadPool()
	{}

	ThreadPool(const ThreadPool<T>& )=delete;

	ThreadPool& operator=(const ThreadPool<T>& )=delete;


private:

	ThreadPool()
	:_num(num)
	,_isrunning(false)
	,_sleepnum(0)
	{
		for(int i=0;i<_num;i++)
		{
			_threads.emplace_back(
				[this](){
					HandlerTask();
				}
			);
		}
	}

	void Start()
	{
		if(_isrunning) return;
		_isrunning=true;
		for(auto&x :_threads)
		{
			x.Start();
		}
	}

	void WakeUpAll()
	{
		{
			LockGuard lockguard(_mutex);
			if (_sleepnum)
				_cond.Broadcast();
		}
	}

	void WakeUpOne()
	{
		_cond.Signal();
		LOG(LogLevel::FATAL)<<"唤醒一个线程";
	}

	static ThreadPool<T>* _th;
	static Mutex _lock;
	std::vector<thread> _threads;
	std::queue<T> _taskq;
	Cond _cond;
	Mutex _mutex;
	bool _isrunning;
	int _sleepnum;
	int _num;
};

template<class T>
ThreadPool<T>* ThreadPool<T>::_th=nullptr;

template<class T>
Mutex ThreadPool<T>::_lock;