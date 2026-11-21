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

	std::string Ip()
	{
		return _ip;
	}

	uint16_t Port()
	{
		return _port;
	}
	
	~InetAddr()
	{}
private:
	struct sockaddr_in& _socket;
	uint16_t _port;
	std::string _ip;
};