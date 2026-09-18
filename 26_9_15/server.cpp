#include "SharedMemory.hpp"
#include"NamedPipeFile.hpp"

#define BUF_SIZE 1024

int main()
{
//     shm ser(CREATOR);
//    // ser.Destory();
//     ser.Create();
//     char* mem=(char*)ser.GetVirualAddress();
//     //ser.Destory();
//     while(true)
//     {
//         std::cin>>mem;
//     }
    // npf a;
    // npf b;
    // a.GetFileInf();
    // b.GetFileInf();
    // int afd=a.

    // npf ser(std::string("fifo"),WRITER);
    // int fd=ser.GetWriteFd();
    // int n=-1;
    // char buf[BUF_SIZE];
    // while(true)
    // {
    //     std::cin>>buf;
    //     n=write(fd,buf,sizeof(buf));
    //     if(strcmp(buf,"end")==0)
    //     {
    //         std::cout<<"write end"<<std::endl;
    //         break;
    //     }
    //     if(n<0)
    //     {
    //         std::cerr<<"write wrong"<<std::endl;
    //         break;
    //     }
    // }

    shm scli(CREATOR);
    scli.Create();
    char* mem=(char*)scli.GetVirualAddress();

    npf ncli("fifo",WRITER);
    int fd=ncli.GetWriteFd();
    if(fd<0)
    {
       return 1;
    }
    int n=-1;
    char buf[BUF_SIZE];
    while(true)
    {
       n=write(fd,buf,sizeof(buf));
       std::cin>>mem;
       if(strcmp(mem,"end")==0)
       {
        std::cout<<"write end"<<std::endl;
        break;
       }
    }
}