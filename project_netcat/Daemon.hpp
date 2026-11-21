#pragma once

#include<iostream>
#include <sys/types.h>         
#include <sys/socket.h>
#include <strings.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <sys/wait.h>
#include <functional>
#include <sys/types.h>
#include <unistd.h>
#include<signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include"log.hpp"

const std::string Dev="/dev/null";

void Daemon()
{
	signal(SIGPIPE,SIG_IGN);
	signal(SIGCHLD,SIG_IGN);

	if(fork()>0)
		exit(0);
	setsid();

	int fd=open(Dev.c_str(),O_RDONLY);
	if(fd<0)
	{
		LOG(LogLevel::ERROR)<<"文件打开失败";
		return;
	}
	else 
	{
		dup2(fd,0);
		dup2(fd,1);
		dup2(fd,2);
		close(fd);
	}
}

