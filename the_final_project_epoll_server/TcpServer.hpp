#pragma once

#include<iostream>
#include<memory>
#include<unordered_map>
#include"Epoller.hpp"
#include"log.hpp"
#include"Connection.hpp"

class TcpServer
{
	const static int REVS_SIZE=256;

	const static int TIMEOUT=1000;
public:
	TcpServer()
	:_epoller_ptr(std::make_unique<Epoller>())
	,_isrunning(false)
	{}

	void Start()
	{	
		if(IsConnectionsEmpty())
		{
			return;
		}
		_isrunning=true;
		while(_isrunning)
		{
			int n=LoopOnce(TIMEOUT);
			Dispatcher(n);
		}
	}

	void AddConnection(std::shared_ptr<Connection> con)
	{
		if(IsConnectionExist(con->GetSockfd()))
		{
			return;
		}
		int event=con->GetEvent();
		int sockfd=con->GetSockfd();
		_epoller_ptr->AddEvent(sockfd,event);
		con->SetTCPServer(this);
		_connections[sockfd]=con;
	}

	void DelConnection(int fd)
	{
		_epoller_ptr->DelEvent(fd);
		_connections.erase(fd);
		close(fd);
	}

	void SetFdEvent(int sockfd,bool read,bool write)
	{
		if(!IsConnectionExist(sockfd))
		{
			LOG(LogLevel::ERROR)<<"fd not exist ,cannot set";
			return;
		}
		uint32_t newevent=(EPOLLET|(read?EPOLLIN:0)|(write?EPOLLOUT:0));
		_connections[sockfd]->SetEvent(newevent);
		_epoller_ptr->ModEvent(sockfd,newevent);
	}

	~TcpServer()
	{}
private:
	bool IsConnectionExist(int sockfd)
	{
		return _connections.find(sockfd)!=_connections.end();
	}

	bool IsConnectionsEmpty()
	{
		return _connections.empty();
	}

	int LoopOnce(int timeout)
	{
		return _epoller_ptr->Wait(_revs,REVS_SIZE,timeout);
	}

	void Dispatcher(int n)
	{
		for (int i = 0; i < n; i++)
		{
			int sockfd = _revs[i].data.fd;
			uint32_t event = _revs->events;
			if (event & EPOLLERR)
				event |= (EPOLLIN | EPOLLHUP);

			if (event & EPOLLHUP)
				event |= (EPOLLIN | EPOLLHUP);

			if (event & EPOLLIN)
			{
				if (IsConnectionExist(sockfd))
					_connections[sockfd]->Recver();
			}

			if (event & EPOLLOUT)
			{
				if (IsConnectionExist(sockfd))
					_connections[sockfd]->Sender();
			}
		}
	}

	std::unique_ptr<Epoller> _epoller_ptr;
	std::unordered_map<int,std::shared_ptr<Connection>> _connections;
	struct epoll_event _revs[REVS_SIZE];
	bool _isrunning;
};