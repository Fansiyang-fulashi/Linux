#include"SharedMemory.hpp"
#include"NamedPipeFile.hpp"

#define BUF_SIZE 1024

int main()
{
   // shm cli(USER);
   // cli.Create();
   // char* mem =(char*)cli.GetVirualAddress();
   // while(true)
   // {
   //    std::cout<<mem<<std::endl;
   //    sleep(2);
   // }
   // npf cli(std::string("fifo"),READER);
   // int fd=cli.GetReadFd();
   // if(fd<0)
   // {
   //    return 1;
   // }
   // int n=-1;
   // char buf[BUF_SIZE];
   // while(true)
   // {
   //    n=read(fd,buf,sizeof(buf));
   //    if(n==0)
   //    {
   //       std::cout<<"read end"<<std::endl;
   //       break;
   //    }
   //    else if(n<0)
   //    {
   //       std::cerr<<"read wrong"<<std::endl;
   //       return 1;
   //    }
   //    buf[n]=0;
   //    std::cout<<buf<<std::endl;
   // }


   shm scli(USER);
   scli.Create();
   char* mem=(char*)scli.GetVirualAddress();

   npf ncli("fifo",READER);
   int fd=ncli.GetReadFd();
   if(fd<0)
   {
      return 1;
   }
   int n=-1;
   char buf[BUF_SIZE];
   while(true)
   {
      n=read(fd,buf,sizeof(buf));
      if(n==0)
      {
         std::cout<<"read end"<<std::endl;
         break;
      }
      else if(n<0)
      {
         std::cerr<<"read wrong"<<std::endl;
         return 1;
      }
      else 
      {
         std::cout<<mem<<std::endl;
      }
   }
}