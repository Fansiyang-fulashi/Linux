#include<iostream>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<cstdio>
#include<cstring>

void ChildWrite(int fd)
{
    char buf[1024];
    int cnt=0;
    while(true)
    {
        snprintf(buf,sizeof(buf),"i am child pid:%d,cnt:%d",getpid(),cnt++);
        write(fd,buf,strlen(buf));
        sleep(1);
    }
}

void FatherRead(int fd)
{
    char buf[1024];
    while(1)
    {
        buf[0]=0;
        size_t n=read(fd,buf,sizeof(buf)-1);
        if(n>0)
        {
            buf[n]=0;
            std::cout<<buf<<std::endl;
        }
    }
}

int main()
{
    //创建管道
    int fds[2]={0};
    int n=pipe(fds);
    if(n<0)
    {
        std::cerr<<"pipe error"<<std::endl;
        return 1;
    }
    //std::cout<<fds[0]<<std::endl;
    //std::cout<<fds[1]<<std::endl;
    pid_t id=fork();
    if(id==0)
    {
        close(fds[0]);
        //
        ChildWrite(fds[1]);

        close(fds[1]);
        exit(0);
    }
    close(fds[1]);
    FatherRead(fds[0]);
    //
    waitpid(id,nullptr,0);
    close(fds[0]);
    return 0;
}