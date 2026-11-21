#pragma once

#include<iostream>
#include <sys/types.h>         
#include <sys/socket.h>
#include <strings.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<functional>
#include"log.hpp"

class InetAddr
{
public:
	InetAddr(struct sockaddr_in& socket)
	:_socket(socket)
	{
		_port=ntohs(_socket.sin_port);
		_ip=inet_ntoa(_socket.sin_addr);
	}

	InetAddr(uint16_t port,std::string ip="0")
	:_port(port)
	,_ip(ip)
	{
		_socket.sin_family=AF_INET;
		_socket.sin_port=htons(_port);
		inet_ntop(AF_INET,&_socket.sin_addr,(char*)_ip.c_str(),sizeof(_ip));
	}

	std::string Ip()
	{
		return _ip;
	}

	uint16_t Port()
	{
		return _port;
	}

	struct sockaddr_in& Socket()
	{
		return _socket;
	}
	
	std::string Name()
	{
		return _ip+":"+std::to_string(_port);
	}

	bool operator==(InetAddr& inetaddr)
	{
		return inetaddr.Ip()==Ip()&&inetaddr.Port()==Port();
	}

	~InetAddr()
	{}
private:
	struct sockaddr_in _socket;
	uint16_t _port;
	std::string _ip;
};