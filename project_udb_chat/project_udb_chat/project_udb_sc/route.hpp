#pragma once

#include<iostream>
#include <sys/types.h>         
#include <sys/socket.h>
#include <strings.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<functional>
#include"log.hpp"
#include"mutex.hpp"
#include"InetAddr.hpp"

class Route
{
public:
	Route()
	{}

	void MessageSend(int sockfd,const std::string& message,InetAddr& inetaddr)
	{
		{
			LockGuard lockguard(_mutex);
			if (!IsExist(inetaddr))
			{
				AddUser(inetaddr);
			}
			if (message == "QUIT")
			{
				DeleteUser(inetaddr);
				std::string quit_mes = inetaddr.Name() + "已经退出" + '\n';
				sendto(sockfd, quit_mes.c_str(), quit_mes.size(), 0, (const sockaddr *)&inetaddr.Socket(), sizeof(inetaddr.Socket()));
				return;
			}
			for (auto &user : _users)
			{
				std::string send_message = inetaddr.Name() + ":" + message + '\n';
				sendto(sockfd, send_message.c_str(), send_message.size(), 0, (const sockaddr *)&inetaddr.Socket(), sizeof(inetaddr.Socket()));
			}
		}
	}

	~Route()
	{
	}
private:
	bool IsExist(InetAddr& inetaddr)
	{
		for(auto& user:_users)
		{
			if(user==inetaddr)
				return true;
		}
		return false;
	}

	void AddUser(InetAddr& inetaddr)
	{
		LOG(LogLevel::INFO)<<"新增一个在线用户"<<inetaddr.Name();
		_users.push_back(inetaddr);
	}

	void DeleteUser(InetAddr& inetaddr)
	{
		auto it=_users.begin();
		while(it!=_users.end())
		{
			if(*it==inetaddr)
			{
				_users.erase(it);
			}
		}
	}

	std::vector<InetAddr> _users;
	Mutex _mutex;

};