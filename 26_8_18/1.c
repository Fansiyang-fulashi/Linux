#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>

char* const p[]={(char*const)"A=100",(char*const)"B=10",NULL};

int main()
{
    char* const argv[]={(char*const)"./exe2",NULL};
    printf("start...\n");
    sleep(2);
    pid_t id=fork();
    if(id==0)
    {
        for(int i=0;p[i];i++)
        {
            putenv(p[i]);
        }
        extern char** environ;
        execve("./exe2",argv,environ);
        return 1;
    }
    waitpid(-1,NULL,0);
    sleep(2);
    printf("end...\n");
    return 0;
}
