#pragma once

#include<iostream>
#include<string>
#include<functional>

class TcpServer;

class Channel;

using handler_t =std::function<std::string(std::string& ch)>;

class Connection
{
public:
	Connection()
	{}

	virtual int Recver()=0;

	virtual int Sender()=0;

	virtual void Excepter()=0;

	void RegistHandler(handler_t handler)
	{
		_handler=handler;
	}

	void SetEvent(const uint32_t event)
	{
		_event=event;
	}

	void SetSockfd(const int& sockfd)
	{
		_sockfd=sockfd;
	}

	uint32_t GetSockfd()
	{
		return _sockfd;
	}

	int GetEvent()
	{
		return _event;
	}

	void SetTCPServer(TcpServer* self)
	{
		_self=self;
	}

	TcpServer* GetTcpServer()
	{
		return _self;
	}

	void SetHandler(handler_t handler)
	{
		_handler=handler;
	}

	~Connection()
	{}
private:
	int _sockfd;
	uint32_t _event;
	TcpServer* _self;
public:
	handler_t _handler;
};