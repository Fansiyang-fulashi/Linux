#pragma once

#include<memory>
#include <sys/select.h>
#include<sys/epoll.h>
#include<sys/poll.h>
#include"Socket.hpp"

class SelectServer
{
private:
	const static int FDS_SIZE=sizeof(fd_set)*8;
	const int default_fd=-1;
public:
	SelectServer(uint16_t port)
	:_listen(std::make_unique<TcpSocket>())
	,_isrunning(false)
	{
		_listen->BuildTcpSocket(port);
		for(int i=0;i<FDS_SIZE;i++)
		{
			_fds[i]=default_fd;
		}
		_fds[0]=_listen->Fd();
	}

	void Start()
	{
		_isrunning=true;
		while(_isrunning)
		{
			fd_set readf;
			fd_set writef;
			FD_ZERO(&readf);
			int maxfd=-1;
			for(int i=0;i<FDS_SIZE;i++)
			{
				if(_fds[i]!=default_fd)
					FD_SET(_fds[i],&readf);
				if(_fds[i]>maxfd)
					maxfd=_fds[i];
			}
			int n=select(maxfd+1,&readf,nullptr,nullptr,nullptr);
			if(n<0)
			{
				LOG(LogLevel::ERROR)<<"select error";
				continue;
			}
			else if(n==0)
			{
				LOG(LogLevel::INFO)<<"time out";
				continue;
			}
			else
			{
				HandlerFd(readf,writef);
			}
		}
	}

	void HandlerFd(fd_set readf,fd_set writef)
	{
		for(int i=0;i<FDS_SIZE;i++)
		{
			if(_fds[i]==default_fd)
				continue;
			if(FD_ISSET(_fds[i],&readf))
			{
				if(_fds[i]==_listen->Fd())
				{
					HandleConnect();
					continue;
				}
				HandleRead(_fds[i],i);
			}
			else if(FD_ISSET(_fds[i],&writef))
			{
				//
			}
		}
	}

	void HandleConnect()
	{
		InetAddr client;
		int sockfd=_listen->Accept(&client);
		if(sockfd<0)
		{
			LOG(LogLevel::INFO)<<"accept error";
			return;
		}

		{
			int pos=0;
			for (;pos<FDS_SIZE;pos++)
			{
				if (_fds[pos] == default_fd)
				{
					_fds[pos] = sockfd;
					break;
				}
			}
			if(pos==FDS_SIZE)
			{
				close(sockfd);
				return;
			}
		}
	}

	void HandleRead(int sockfd,int i)
	{
		char buf[1024];
		int n=read(sockfd,buf,sizeof(buf));
		if(n>0)
		{
			LOG(LogLevel::INFO)<<"client said:"<<buf;
		}
		else if(n==0)
		{
			LOG(LogLevel::INFO)<<"client over";
			_fds[i]=default_fd;
			close(sockfd);
		}
		else
		{
			LOG(LogLevel::INFO)<<"client have a problem";
			_fds[i]=default_fd;
			close(sockfd);
		}
	}

	void PrintFds()
	{
		for(int i=0;i<FDS_SIZE;i++)
		{
			std::cout<<_fds[i]<<" ";
		}
	}

	~SelectServer()
	{}
private:
	std::unique_ptr<Socket> _listen;
	bool _isrunning;
	int _fds[FDS_SIZE];
};

class PoolServer
{
private:
	const static int FDS_SIZE=4096;
	const int default_fd=-1;
public:
	PoolServer(uint16_t port)
	:_listen(std::make_unique<TcpSocket>())
	,_isrunning(false)
	{
		_listen->BuildTcpSocket(port);
		for(int i=0;i<FDS_SIZE;i++)
		{
			_fds[i].fd=default_fd;
			_fds[i].events=0;
			_fds[i].revents=0;
		}
		_fds[0].fd=_listen->Fd();
		_fds[0].events=POLLIN;
	}

	void Start()
	{
		_isrunning=true;
		while(_isrunning)
		{
			int n=poll(_fds,FDS_SIZE,0);
			if(n<0)
			{
				LOG(LogLevel::ERROR)<<"select error";
				continue;
			}
			else if(n==0)
			{
				//LOG(LogLevel::INFO)<<"time out";
				continue;
			}
			else
			{
				HandlerFd();
			}
		}
	}

	void HandlerFd()
	{
		for(int i=0;i<FDS_SIZE;i++)
		{
			if(_fds[i].fd==default_fd)
				continue;
			if(_fds[i].events&POLLIN)
			{
				if(_fds[i].fd==_listen->Fd())
				{
					HandleConnect();
					continue;
				}
				HandleRead(i);
			}
			else if(_fds[i].events&POLLOUT)
			{
				//
			}
		}
	}

	void HandleConnect()
	{
		InetAddr client;
		int sockfd=_listen->Accept(&client);
		if(sockfd<0)
		{
			LOG(LogLevel::INFO)<<"accept error";
			return;
		}

		{
			int pos=0;
			for (;pos<FDS_SIZE;pos++)
			{
				if (_fds[pos].fd == default_fd)
				{
					_fds[pos].fd = sockfd;
					_fds[pos].events=POLLIN;
					break;
				}
			}
			if(pos==FDS_SIZE)
			{
				close(sockfd);
				return;
			}
		}
	}

	void HandleRead(int i)
	{
		char buf[1024];
		int n=read(_fds[i].fd,buf,sizeof(buf));
		if(n>0)
		{
			LOG(LogLevel::INFO)<<"client said:"<<buf;
		}
		else if(n==0)
		{
			LOG(LogLevel::INFO)<<"client over";
			close(_fds[i].fd);
			_fds[i].fd=default_fd;
		}
		else
		{
			LOG(LogLevel::INFO)<<"client have a problem";
			close(_fds[i].fd);
			_fds[i].fd=default_fd;
		}
	}

	void PrintFds()
	{
		for(int i=0;i<FDS_SIZE;i++)
		{
			std::cout<<_fds[i].fd<<" ";
		}
	}

	PoolServer()
	{}
private:
	std::unique_ptr<Socket> _listen;
	bool _isrunning;
	struct pollfd _fds[FDS_SIZE];
};

class EpollServer
{
private:
	const static int default_epfd=-1;
	const static int EPS_SIZE=256;
public:
	EpollServer(uint16_t port)
	:_listen(std::make_unique<TcpSocket>())
	,_isrunning(false)
	,_epfd(default_epfd)
	{
		_listen->BuildTcpSocket(port);
		_epfd=epoll_create(256);
		if(_epfd<0)
		{
			LOG(LogLevel::FATAL)<<"epoll create error";
			exit(1);
		}
		struct epoll_event ev;
		ev.events=EPOLLIN;
		ev.data.fd=_listen->Fd();
		int n=epoll_ctl(_epfd,EPOLL_CTL_ADD,_listen->Fd(),&ev);
		if(n<0)
		{
			LOG(LogLevel::FATAL)<<"epoll ctrl error";
			exit(1);
		}
	}

	void Start()
	{
		_isrunning=true;
		while(_isrunning)
		{
			int n=epoll_wait(_epfd,_eps,EPS_SIZE,0);
			if(n<0)
			{
				LOG(LogLevel::ERROR)<<"select error";
				continue;
			}
			else if(n==0)
			{
				//LOG(LogLevel::INFO)<<"time out";
				continue;
			}
			else
			{
				HandlerFd(n);
			}
		}
	}

	void HandlerFd(int n)
	{
		for(int i=0;i<n;i++)
		{
			uint32_t event=_eps[i].events;
			int fd=_eps[i].data.fd;
			if(event&EPOLLIN)
			{
				if(fd==_listen->Fd())
				{
					HandleConnect();
					continue;
				}
				HandleRead(i);
			}
			else if(event&EPOLLOUT)
			{
				//
			}
		}
	}

	void HandleConnect()
	{
		InetAddr client;
		int sockfd=_listen->Accept(&client);
		if(sockfd<0)
		{
			LOG(LogLevel::INFO)<<"accept error";
			return;
		}
		epoll_event ev;
		ev.events=EPOLLIN;
		ev.data.fd=sockfd;
		int n=epoll_ctl(_epfd,EPOLL_CTL_ADD,sockfd,&ev);
	}

	void HandleRead(int i)
	{
		char buf[1024];
		int fd=_eps[i].data.fd;
		int n=read(fd,buf,sizeof(buf));
		if(n>0)
		{
			LOG(LogLevel::INFO)<<"client said:"<<buf;
		}
		else if(n==0)
		{
			LOG(LogLevel::INFO)<<"client over";
			epoll_ctl(_epfd,EPOLL_CTL_DEL,fd,nullptr);
			close(fd);
		}
		else
		{
			LOG(LogLevel::INFO)<<"client have a problem";
			close(fd);
		}
	}

	void PrintFds()
	{
		
	}

	EpollServer()
	{
		if(_epfd>0)
		close(_epfd);
	}
private:
	std::unique_ptr<Socket> _listen;
	bool _isrunning;
	int _epfd;
	epoll_event _eps[EPS_SIZE];
};