#include"comm.hpp"

int main()
{
    int fd=open(FILE_NAME,O_CREAT|O_WRONLY|O_TRUNC);
    while(true)
    {
        char buf[BUF_SIZE];
        std::cin>>buf;
        int writer=write(fd,buf,sizeof(buf));
    }
    return 0;
}