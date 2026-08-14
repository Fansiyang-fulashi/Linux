#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

int main()
{
    pid_t id=fork();
    if(id==0)
    {
        //son
        while(1)
        {
            printf("i am son,pid:%d\n",getpid());
            sleep(1);
        }
    }
    else if(id<0)
    {
        perror("error");
        return 0;
    }
    else
    {
        //father
        for(int i=0;i<5;i++)
        {
            sleep(1);
            printf("i am father,pid:%d\n",getpid());
        }
    }
    return 0;
}
