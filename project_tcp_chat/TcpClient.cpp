#include<iostream>
#include <sys/types.h>         
#include <sys/socket.h>
#include <strings.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<functional>
#include"log.hpp"
#include"comm.hpp"
#include"InetAddr.hpp"
#include"memory"

int main(int argc,char*argv[])
{
	if(argc!=3)
	{
		LOG(LogLevel::ERROR)<<"输入错误";
		exit(ERROR::USE_ERROR);
	}
	std::string serip=argv[1];
	uint16_t serport=std::stoi(argv[2]);
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		LOG(LogLevel::ERROR) << "套接字创建错误";
		exit(ERROR::SOKET_ERROR);
	}
	LOG(LogLevel::INFO) << "套接字创建成功";
	InetAddr inet(serport,serip);
	int n=connect(sockfd,inet.SockAddr(),inet.Len());
	if(n<0)
	{
		LOG(LogLevel::ERROR) << "连接错误";
		exit(ERROR::CONNECT_ERROR);
	}
	LOG(LogLevel::ERROR) << "连接成功";

	while(true)
	{
		std::string line;
		std::cout<<"client enter# ";
		std::getline(std::cin,line);
		write(sockfd,line.c_str(),line.size());
		char buf[1024];
		int n=read(sockfd,buf,sizeof(buf)-1);
		if(n>0)
		{
			buf[n]=0;
			std::cout<<buf<<std::endl;
		}
	}
	close(sockfd);
}