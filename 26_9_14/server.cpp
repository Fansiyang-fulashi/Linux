#include"comm.hpp"

int main()
{
    umask(0);
    int mkfifor=mkfifo(FILE_NAME,0666);
    if(mkfifor!=0)
    {
        std::cerr<<"the mkfifo was wrong!!!"<<std::endl;
        return 1;
    }
    int fd=open(FILE_NAME,O_RDONLY);
    if(fd<0)
    {
        std::cerr<<"open was wrong!!!"<<std::endl;
        return 1;
    }
    char buf[BUF_SIZE];
    while(true)
    {
        int readr=read(fd,buf,sizeof(buf));
        if(readr<0)
        {
            std::cerr<<"read was wrong!!!"<<std::endl;
          return 1;
        }
        if(readr>0)
        {
            buf[readr]=0;
            std::cout<<buf<<std::endl;
        }
    }
    return 0;
}