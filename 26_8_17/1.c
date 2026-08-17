#include <stdio.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
    pid_t id=fork();
    if(id==0)
    {
        for(int i=0;i<5;i++)
        {
            printf("我是子进程,pid:%d\n",getpid());
        }
        //int* p=NULL;
        //*p=1;
        return 1;
    }
    //pid_t rid=wait(NULL);
    int status=0;
    pid_t rid=waitpid(id,&status,0);
    if(rid>0)
    {
        printf("rid:%d...%d...%d\n",rid,WEXITSTATUS(status),status&0x7F);
    }
    return 0;
   // FILE* pf=fopen("jjj.txt","r");
   // if(pf==NULL)
   // {
   //     return errno;
   // }
   // fclose(pf);
}

