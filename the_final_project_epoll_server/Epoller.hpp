#pragma once

#include<iostream>
#include<sys/epoll.h>
#include"log.hpp"
#include"comm.hpp"

class Epoller
{
	const static int default_epfd=-1;
public:
	Epoller()
	:_epfd(default_epfd)
	{
		_epfd=epoll_create(256);
		if(_epfd<0)
		{
			LOG(LogLevel::FATAL)<<"epoll create error";
			exit(EPOLL_CREATE_ERROR);
		}
	}

	void AddEvent(int fd,int event)
	{
		epoll_event ev;
		ev.events=event;
		ev.data.fd=fd;
		int n=epoll_ctl(_epfd,EPOLL_CTL_ADD,fd,&ev);
		if(n<0)
		{
			LOG(LogLevel::ERROR)<<"epoll add event error";
		}
	}

	void DelEvent(int fd)
	{
		int n=epoll_ctl(_epfd,EPOLL_CTL_DEL,fd,nullptr);
		if(n<0)
		{
			LOG(LogLevel::ERROR)<<"epoll del event error";
		}
	}

	void ModEvent(int fd,uint32_t newevent)
	{
		epoll_event ev;
		ev.events=newevent;
		ev.data.fd=fd;
		int n=epoll_ctl(_epfd,EPOLL_CTL_MOD,fd,&ev);
		if(n<0)
		{
			LOG(LogLevel::ERROR)<<"epoll mod event error";
		}
	}

	int Wait(struct epoll_event revs[],int size,int timeout)
	{
		int n=epoll_wait(_epfd,revs,size,timeout);
		if(n<0)
		{
			LOG(LogLevel::ERROR)<<"epoll wait error";
		}
		else if(n==0)
		{
			//LOG(LogLevel::ERROR)<<"time out";
		}
		return n;
	}

	~Epoller()
	{}
private:
	int _epfd;
};