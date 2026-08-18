#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
    printf("start...\n");
    sleep(2);
    pid_t id=fork();
    if(id==0)
    {
        execl("../26_8_17/exe","exe",NULL);
        return 1;
    }
    waitpid(-1,NULL,0);
    sleep(2);
    printf("end...\n");
    return 0;
}
