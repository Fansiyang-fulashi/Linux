#pragma once

#include<iostream>
#include<pthread.h>
#include<string>
#include<functional>
#include<unistd.h>


class thread
{
	using func_t =std::function<void()>;
public:
	thread(func_t func)
	:_tid(0)
	,_isdetach(false)
	,_isrunning(false)
	,_func(func)
	{
	}

	~thread()
	{}

	static void* Routine(void* args)
	{
		thread* self=static_cast<thread*>(args);
		self->_func();
		return nullptr;
	}

	bool Start()
	{
		if(_isrunning)
		return false;
		int n=pthread_create(&_tid,nullptr,Routine,this);
		if(n!=0)
		{
			
			return false;
		}
		else 
		{
			
		}
		if(_isdetach)
		Detach();
		_isrunning=true;
		return true;
	}

	void Detach()
	{
		if(!_isdetach&&_tid!=0)
		pthread_detach(_tid);
		_isdetach=true;
	}

	void Stop()
	{
		if(!_isrunning)
		return;
		int n=pthread_cancel(_tid);
		if(n!=0)
		{
			
			return;
		}
		else 
		{
			
		}
	}

	bool Join()
	{
		int n=pthread_join(_tid,&_return_num);
		if(n!=0)
		{
			
			return false;
		}
		else
		{
			
		}
		return true;
	}

	pthread_t ID()
	{
		return _tid;
	}
private:
	pthread_t _tid;
	bool _isdetach;
	bool _isrunning;
	void* _return_num;
	func_t _func;
};