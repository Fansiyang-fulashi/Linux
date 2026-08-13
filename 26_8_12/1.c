#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

int main()
{
    pid_t id= fork();
    if(id<0)
    {
        perror("错误\n");
        return -1;
    }
    else if(id==0)
    {
         while(1)
        {
            sleep(1);
            printf("我是一个子进程,我的pid是%d\n",getpid());
        }
    }
    else
    {
         while(1)
        {
            sleep(1);
            printf("我是一个父进程,我的pid是%d\n",getpid());
        }
    }
    return 0;
}
