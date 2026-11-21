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
#include"memory"
#include"threadpool.hpp"

const int backlog=6;

using task_t =std::function<void()>;

using func_t =std::function<const std::string(const std::string&)>;

class TcpServer;

class ThreadData
{
public:
	ThreadData(int sockfd,InetAddr &inet,TcpServer* tcpserver)
	:_sockfd(sockfd)
	,_inet(inet)
	,_tcpserver(tcpserver)
	{}

	int _sockfd;
	InetAddr &_inet;
	TcpServer* _tcpserver;
};

class TcpServer:private NoCopy
{
public:
	TcpServer(uint16_t port,func_t func)
	:_port(port)
	,_isrunning(false)
	,_func(func)
	{
		_listen_sockfd=socket(AF_INET,SOCK_STREAM,0);
		if(_listen_sockfd<0)
		{
			LOG(LogLevel::ERROR)<<"套接字创建错误";
			exit(ERROR::SOKET_ERROR);
		}
		LOG(LogLevel::INFO)<<"套接字创建成功";
		InetAddr inetaddr(port);
		int n=bind(_listen_sockfd,inetaddr.SockAddr(),inetaddr.Len());
		if(n<0)
		{
			LOG(LogLevel::ERROR)<<"绑定失败";
			exit(ERROR::BIND_ERROR);
		}
		LOG(LogLevel::INFO)<<"绑定成功";
		n=listen(_listen_sockfd,backlog);
		if(n<0)
		{
			LOG(LogLevel::ERROR)<<"监听失败";
			exit(ERROR::LISTEN_ERROR);
		}
		LOG(LogLevel::ERROR)<<"监听成功";
	}

	void Init()
	{}

	void Start()
	{
		_isrunning=true;
		while(_isrunning)
		{
			struct sockaddr_in peer;
			socklen_t len=sizeof(peer);
			int sockfd=accept(_listen_sockfd,(struct sockaddr*)&peer,&len);
			if(sockfd<0)
			{
				LOG(LogLevel::WARNING)<<"接受错误";
				continue;
			}
			InetAddr inetaddr(peer);
			LOG(LogLevel::WARNING)<<"接受成功"<<" 端口号："<<inetaddr.Port()<<" ip:"<<inetaddr.Ip();
			//《1》
			// int pid=fork();
			// if(pid<0)
			// {
			// 	LOG(LogLevel::FATAL)<<"进程创建失败";
			// 	exit(ERROR::FORK_ERROR);
			// }
			// else if(pid==0)
			// {
			// 	close(_listen_sockfd);
			// 	if(fork()>0)
			// 		exit(ERROR::OK);
			// 	Server(sockfd,inetaddr);
			// 	exit(ERROR::OK);
			// }
			// else
			// {
			// 	close(sockfd);
			// 	waitpid(pid,nullptr,0);
			// }

			//《2》
			ThreadData* _th=new ThreadData(sockfd,inetaddr,this);
			pthread_t pid;
			pthread_create(&pid,nullptr,Routine,_th);

			//《3》
			// ThreadPool<task_t>::GetInstance()->Push([this,&sockfd,&inetaddr](){
			// 	this->Server(sockfd,inetaddr);
			// });
		}
		_isrunning=false;
	}

	void Server(int sockfd,InetAddr inet)
	{
		while (true)
		{
			char buf[1024];
			int n = read(sockfd, buf, sizeof(buf));
			if(n<0)
			{
				LOG(LogLevel::ERROR)<<"读取失败";
				return;
			}
			else if(n==0)
			{
				LOG(LogLevel::INFO)<<"客户端退出";
				close(sockfd);
				return;
			}
			buf[n] = 0;
			std::string sers=_func(buf);
			// LOG(LogLevel::INFO) << inet.Ip() << " said# " << buf;
			// std::string sers = "server said# hello client";
			write(sockfd, sers.c_str(), sers.size());
		}
	}

	static void* Routine(void* argv)
	{
		pthread_detach(pthread_self());
		ThreadData* _th=(ThreadData*)argv;
		_th->_tcpserver->Server(_th->_sockfd,_th->_inet);
		return nullptr;
	}

	~TcpServer()
	{}
private:

	int _listen_sockfd;
	uint16_t _port;
	bool _isrunning;
	func_t _func;
};