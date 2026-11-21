#pragma once

#include<functional>
#include"Connection.hpp"
#include"InetAddr.hpp"
#include"comm.hpp"

class Channel;

using handler_t =std::function<std::string(std::string& ch)>;

class Channel:public Connection
{
	const static int BUF_SIZE =1024;
public:
	Channel(int sockfd,InetAddr& client)
	:_client(client)
	{
		SetSockfd(sockfd);
		SetNonBlock(sockfd);
	}

	int Recver() override
	{
		char buf[BUF_SIZE];
		while(true)
		{
			int n=recv(GetSockfd(),buf,sizeof(buf),0);
			if(n>0)
			{
				buf[n]=0;
				_inspace+=buf;
			}
			else if(n==0)
			{
				Excepter();
				break;
			}
			else
			{
				if (errno == EAGAIN || errno == EWOULDBLOCK)
					break;
				else if (errno == EINTR)
					continue;
				else
				{
					Excepter();
					break;
				}
			}
		}
		if(!_inspace.empty())
			_outspace+=_handler(_inspace);
		if(!_outspace.empty())
			Sender();
	}

	int Sender() override
	{
		while(!_outspace.empty())
		{
			ssize_t n=send(GetSockfd(),_outspace.c_str(),_outspace.size(),0);
			if(n>0)
			{
				_outspace.erase(0,n);
			}
			else 
			{
				if (errno == EAGAIN || errno == EWOULDBLOCK)
					break;
				else if (errno == EINTR)
					continue;
				else
				{
					Excepter();
					break;
				}
			}
		}

		if(!_outspace.empty())
		{
			GetTcpServer()->SetFdEvent(GetSockfd(),true,true);
		}
		else 
		{
			GetTcpServer()->SetFdEvent(GetSockfd(),true,false);
		}
	}

	void Excepter()override
	{
		GetTcpServer()->DelConnection(GetSockfd());
	}

	std::string& Inspace()
	{
		return _inspace;
	}

	void SetOutspce(std::string& outspace)
	{
		_outspace=outspace;
	}

	~Channel()
	{}
private:
	std::string _inspace;
	std::string _outspace;
	InetAddr _client;
};