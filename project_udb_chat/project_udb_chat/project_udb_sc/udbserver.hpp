#pragma once

#include<iostream>
#include <sys/types.h>         
#include <sys/socket.h>
#include <strings.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<functional>
#include"log.hpp"
#include"InetAddr.hpp"

using func_t = std::function<void(int sockfd,const std::string&,InetAddr&)>;

class UdbServer
{
public:
	UdbServer(uint16_t port,func_t fun)
	:_port(port)
	,_isrunning(false)
	,_fun(fun)
	{
		_sockfd=socket(AF_INET,SOCK_DGRAM,0);
		if(_sockfd<0)
		{
			LOG(LogLevel::FATAL)<<"套接字文件创建失败";
			exit(1);
		}
		LOG(LogLevel::FATAL)<<"套接字文件创建成功";
		struct sockaddr_in local;
		bzero(&local,sizeof(local));
		local.sin_family=AF_INET;
		local.sin_port=htons(_port);
		local.sin_addr.s_addr=INADDR_ANY;
		int n=bind(_sockfd,(struct sockaddr*)&local,sizeof(local));
		if(n<0)
		{
			LOG(LogLevel::FATAL)<<"绑定失败";
			exit(2);
		}
		LOG(LogLevel::FATAL)<<"绑定成功";
	}

	~UdbServer()
	{}

	void Init()
	{}

	void Start()
	{
		_isrunning=true;
		while(_isrunning)
		{
			char buf[1024];
			struct sockaddr_in peer;
			bzero(&peer,sizeof(peer));
			socklen_t len=sizeof(peer);
			ssize_t n=recvfrom(_sockfd,buf,sizeof(buf),0,(struct sockaddr*)&peer,&len);
			if(n>0)
			{
				InetAddr client(peer);
				buf[n]=0;
				_fun(_sockfd,buf,client);
			}
		}
	}
	
private:
	int _sockfd;
	uint16_t _port;
	bool _isrunning;
	func_t _fun;
};