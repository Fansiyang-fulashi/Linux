#include<iostream>
#include <unistd.h>
#include <fcntl.h>

void SetFdNonblock(int fd)
{
	int fl=fcntl(fd,F_GETFL);
	if(fl<0)
	{
		perror("fl");
		return;
	}
	fcntl(fd,F_SETFL,fl|O_NONBLOCK);
}

int main()
{
	SetFdNonblock(0);
	char buf[1024];
	while(true)
	{
		ssize_t n=read(0,buf,sizeof(buf));
		if(n>0)
		{
			buf[n-1]=0;
			std::cout<<buf<<std::endl;
		}
		else if(n<0)
		{
			if(errno==EAGAIN||errno==EWOULDBLOCK)
			{
				std::cout<<"数据还没准备好"<<std::endl;
				sleep(1);
			}

			else if(errno==EINTR)
			{
				std::cout<<"出错了"<<std::endl;
				sleep(1);
			}
		}
	}
}