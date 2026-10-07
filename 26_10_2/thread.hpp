#pragma once

#include"comm.hpp"

template<class T>
class thread
{
	using func_t =std::function<void(T&)>;
public:
	thread(std::string name,func_t func,const T& data)
	:_tid(0)
	,_isdetach(false)
	,_isrunning(false)
	,_name(name)
	,_func(func)
	,_data(data)
	{
	}

	~thread()
	{}

	static void* Routine(void* args)
	{
		thread<T>* self=static_cast<thread<T>*>(args);
		self->_func(self->_data);
		return nullptr;
	}

	bool Start()
	{
		if(_isrunning)
		return false;
		int n=pthread_create(&_tid,nullptr,Routine,this);
		if(n!=0)
		{
			std::cout<<"pthread create:"<<errno<<std::endl;
			return false;
		}
		else 
		{
			std::cout<<"create ok"<<std::endl;
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
			std::cout<<"pthread cancel:"<<errno<<std::endl;
			return;
		}
		else 
		{
			std::cout<<"cancel ok"<<std::endl;
		}
	}

	bool Join()
	{
		int n=pthread_join(_tid,&_return_num);
		if(n!=0)
		{
			std::cout<<"pthread join:"<<errno<<std::endl;
			return false;
		}
		else
		{
			std::cout<<"join ok"<<std::endl;
		}
		return true;
	}
private:
	pthread_t _tid;
	std::string _name;
	bool _isdetach;
	bool _isrunning;
	void* _return_num;
	func_t _func;
	T _data;
};