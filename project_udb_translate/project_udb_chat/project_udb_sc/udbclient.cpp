#include<iostream>
#include"log.hpp"
#include <sys/types.h>         
#include <sys/socket.h>
#include <strings.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main(int argc,char*argv[])
{
	Enable_Console_Log_Stategy();
	if(argc!=3)
	{
		LOG(LogLevel::ERROR)<<"使用错误";
		return 1;
	}
	std::string ip=argv[1];
	uint16_t port=std::stoi(argv[2]);

	int _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (_sockfd < 0)
	{
		LOG(LogLevel::FATAL) << "套接字文件创建失败";
		exit(1);
	}
	LOG(LogLevel::FATAL) << "套接字文件创建成功";

	struct sockaddr_in local;
	bzero(&local, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip.c_str());

	while(true)
	{
		std::string input;
		std::cout<<"请输入:";
		std::getline(std::cin,input);

		int m=sendto(_sockfd,input.c_str(),input.size(),0,(struct sockaddr*)&local,sizeof(local));

		char buf[1024];
		struct sockaddr_in peer;
		socklen_t len=sizeof(peer);
		ssize_t n=recvfrom(_sockfd,buf,sizeof(buf),0,(struct sockaddr*)&peer,&len);
		if (n > 0)
		{
			buf[n] = 0;
			//LOG(LogLevel::FATAL) << "接收消息成功，消息为：" << buf;
			LOG(LogLevel::FATAL) << "翻译为：" << buf;
		}
	}
}