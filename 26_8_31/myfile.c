#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

int main()
{
    umask(0);
    int fd=open("log.txt",O_CREAT|O_WRONLY,0666);
    if(fd<0)
        return 1;
    const char* buf="renfne\n";
    write(fd,buf,strlen(buf));
    close(fd);
    return 0;
}
