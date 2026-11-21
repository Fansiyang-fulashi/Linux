#pragma once

#include"Socket.hpp"

using tcp_ioserver_t = std::function<void(std::shared_ptr<Socket> sock,InetAddr client)>;

class TcpServer
{
public:
	TcpServer(uint16_t port,tcp_ioserver_t server)
	:_port(port)
	,_socketptr(std::make_unique<TcpSocket>())
	,_server(server)
	,_isrunning(false)
	{
		_socketptr->BuildTcpSocket(port);
	}

	void Start()
	{
		_isrunning=true;
		while(_isrunning)
		{
			InetAddr inetaddr;
			auto tcpsock=_socketptr->Accept(&inetaddr);
			if(tcpsock==nullptr)
			{
				LOG(LogLevel::INFO)<<"接收失败";
				continue;
			}
			LOG(LogLevel::INFO)<<"接收成功";
			pid_t pid=fork();
			if(pid<0)
			{
				LOG(LogLevel::FATAL)<<"进程创建失败";
				exit(ERROR::FORK_ERROR);
			}
			else if(pid==0)
			{
				_socketptr->Close();
				if(fork()>0)
					exit(ERROR::OK);
				_server(tcpsock,inetaddr);
				exit(ERROR::OK);
			}
			else
			{
				tcpsock->Close();
				waitpid(pid,nullptr,0);
			}
		}
		_isrunning=false;
	}

	~TcpServer()
	{}
private:
	uint16_t _port;
	std::unique_ptr<Socket> _socketptr;
	tcp_ioserver_t _server;
	bool _isrunning;
};