#pragma once

#include<iostream>
#include<sys/epoll.h>
#include"TcpServer.hpp"
#include"Connection.hpp"
#include"Channel.hpp"
#include"Socket.hpp"

class Listener:public Connection
{
	const static int default_port=8080;
public:
	Listener(int port=default_port)
	:_port(port)
	,_socket(std::make_unique<TcpSocket>())
	{
		_socket->BuildTcpSocket(port);
		SetEvent(EPOLLIN|EPOLLET);
		SetNonBlock(_socket->Fd());
		SetSockfd(_socket->Fd());
	}

	int Recver()override
	{
		InetAddr client;
		while(true)
		{
			int sockfd=_socket->Accept(&client);
			if(sockfd==TcpSocket::ACCEPT_ERROR)
				break;
			else if(sockfd==TcpSocket::ACCEPT_CONTINUE)
				continue;
			else if(sockfd==TcpSocket::ACCEPT_DONE)
				break;
			else
			{
				std::shared_ptr<Connection> con=std::make_shared<Channel>(sockfd,client);
				con->SetEvent(EPOLLIN|EPOLLET);
				if(_handler!=nullptr)
				{
					con->SetHandler(_handler);
				}
				GetTcpServer()->AddConnection(con);
			}
		}
	}

	int Sender()override
	{}

	void Excepter()override
	{}

	~Listener()
	{}
private:
	int _port;
	std::unique_ptr<Socket> _socket;
};