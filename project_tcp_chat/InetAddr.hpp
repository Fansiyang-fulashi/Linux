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

	InetAddr(uint16_t port,std::string ip)
	:_port(port)
	,_ip(ip)
	{
		_socket.sin_family=AF_INET;
		_socket.sin_port=htons(_port);
		inet_pton(AF_INET,_ip.c_str(),&_socket.sin_addr);
	}

	InetAddr(uint16_t port)
	:_port(port)
	{
		_socket.sin_family=AF_INET;
		_socket.sin_port=htons(_port);
		_socket.sin_addr.s_addr=INADDR_ANY;
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

	struct sockaddr* SockAddr()
	{
		return (sockaddr*)&_socket;
	}

	socklen_t Len()
	{
		return sizeof(_socket);
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