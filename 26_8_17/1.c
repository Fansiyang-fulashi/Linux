#include <stdio.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

typedef void(*func_t)();

#define NUM 5

func_t function[NUM+1];

void DownLoad()
{
    printf("please waiting download....\n");
    sleep(3);
    printf("time out 3s\n");
}

void Flush()
{
    printf("please waiting flush....\n");
    sleep(2);
    printf("time out 2s\n");
}

void Log()
{
    printf("please waiting log....\n");
    sleep(1);
    printf("time out 1s\n");
}

void Init()
{
    printf("please waiting init....\n");
    sleep(4);
    printf("time out 4s\n");
}

void Start()
{
    printf("please waiting start....\n");
    sleep(1);
    printf("time out 1s\n");
}

void registfunction(func_t f[],func_t t)
{
    for(int i=0;i<NUM;i++)
    {
        if(f[i]==NULL)
        {
            f[i]=t;
            break;
        }
    }
}

int main()
{
    registfunction(function,DownLoad);
    registfunction(function,Flush);
    registfunction(function,Log);
    registfunction(function,Init);
    registfunction(function,Start);
    pid_t id=fork();
    if(id==0)
    {
        sleep(3);
        for(int i=0;i<5;i++)
        {
            printf("child going...\n");
        }
        sleep(30);
        //int* p=NULL;
        //*p=1;
        return 1;
    }


    while(1)
    {
        int status=0;
        pid_t rid=waitpid(id,&status,WNOHANG);
        if(rid>0)
        {
            printf("success!!!\n");
            break;
        }
        else if(rid==0)
        {
            printf("continue...\n");
            //
            for(int i=0;;i++)
            {
                if(function[i]!=NULL)
                {
                    function[i]();
                }
                else
                {
                    break;
                }
            }
            sleep(1);
        }
        else 
        {
            printf("ERROR!!!\n");
            break;
        }
    }
    return 0;
    ////pid_t rid=wait(NULL);
    //Int status=0;
    //Pid_t rid=waitpid(id,&status,0);
    //If(rid>0)
    //{
    //    printf("rid:%d...%d...%d\n",rid,WEXITSTATUS(status),status&0x7F);
    //}
    //Return 0;
   //// FILE* pf=fopen("jjj.txt","r");
   //// if(pf==NULL)
   //// {
   ////     return errno;
   //// }
   //// fclose(pf);
}

