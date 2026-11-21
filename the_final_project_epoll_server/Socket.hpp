#pragma once

#include<iostream>
#include <sys/types.h>         
#include <sys/socket.h>
#include <strings.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <sys/wait.h>
#include <functional>
#include <sys/types.h>
#include <unistd.h>
#include"log.hpp"
#include"comm.hpp"
#include"InetAddr.hpp"

const int gbacklog=6;

class Socket
{
public:
	virtual ~Socket()=0;
	virtual void Create()=0;
	virtual void Bind(uint16_t port)=0;
	virtual void Listen(int backlog)=0;
	virtual int Accept(InetAddr* client)=0;
	virtual void Close()=0;
	virtual int Read(std::string* out)=0;
	virtual int Send(const std::string& out)=0;
	virtual int Connect(std::string ip,uint16_t port)=0;
	virtual int Fd()=0;

	void BuildTcpSocket(uint16_t port,int backlog=gbacklog)
	{
		Create();
		Bind(port);
		Listen(backlog);
	}

	void BuildClientTcpSocket()
	{
		Create();
	}
};

class TcpSocket:public Socket
{
public:
	TcpSocket(int sockfd=-1)
	:_listen_sockfd(sockfd)
	{}

	~TcpSocket() override
	{}

	void Create() override
	{
		_listen_sockfd=socket(AF_INET,SOCK_STREAM,0);
		if(_listen_sockfd<0)
		{
			LOG(LogLevel::ERROR)<<"套接字创建错误";
			exit(ERROR::SOKET_ERROR);
		}
		LOG(LogLevel::INFO)<<"套接字创建成功";
	}

	void Bind(uint16_t port) override
	{
		InetAddr inetaddr(port);
		int n=bind(_listen_sockfd,inetaddr.SockAddr(),inetaddr.Len());
		if(n<0)
		{
			LOG(LogLevel::ERROR)<<"绑定失败";
			exit(ERROR::BIND_ERROR);
		}
		LOG(LogLevel::INFO)<<"绑定成功";
	}

	void Listen(int backlog) override
	{
		int n=listen(_listen_sockfd,backlog);
		if(n<0)
		{
			LOG(LogLevel::ERROR)<<"监听失败";
			exit(ERROR::LISTEN_ERROR);
		}
		LOG(LogLevel::ERROR)<<"监听成功";
	}

	const static int ACCEPT_DONE=-1;
	const static int ACCEPT_CONTINUE=-2;
	const static int ACCEPT_ERROR=-3;


	int Accept(InetAddr* client) override
	{
		struct sockaddr_in peer;
		socklen_t len = sizeof(peer);	
		int sockfd = accept(_listen_sockfd, (struct sockaddr *)&peer, &len);
		if (sockfd < 0)
		{
			if(errno==EAGAIN||errno==EWOULDBLOCK)
			{
				return ACCEPT_DONE;
			}
			else if(errno==EINTR)
			{
				return ACCEPT_CONTINUE;
			}
			else 
			{
				LOG(LogLevel::ERROR)<<"accept error";
				return ACCEPT_ERROR;
			}
		}
		InetAddr inetaddr(peer);
		LOG(LogLevel::WARNING) << "接受成功" << " 端口号：" << inetaddr.Port() << " ip:" << inetaddr.Ip();
		*client=inetaddr;
		return sockfd;
	}

	int Read(std::string* out) override
	{
		char buf[1024];
		int n=read(_listen_sockfd,buf,sizeof(buf));
		*out+=buf;
		return n;
	}

	int Send(const std::string& out) override
	{
		return send(_listen_sockfd,out.c_str(),out.size(),0);
	}

	int Connect(std::string ip,uint16_t port) override
	{
		InetAddr addr(port,ip);
		return connect(_listen_sockfd,addr.SockAddr(),addr.Len());
	}

	int Fd() override
	{
		return _listen_sockfd;
	}

	void Close() override
	{
		if(_listen_sockfd>0)
			close(_listen_sockfd);
	}
private:
	int _listen_sockfd;
};

Socket::~Socket()
{}