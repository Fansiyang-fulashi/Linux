#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<limits.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

//打印命令行相关参数
#define COMMAND_SIZE_MAX 1024
#define COMMAND_PROMAT_SIZE_MAX 256
#define FORMAT "%s@%s:%s$ "

//命令行分析参数
#define MAXARGC 128
char* argv[MAXARGC];
int argc=0;

const char* GetUserName()
{
    const char* username=getenv("USER");
    return username==NULL?"None":username;
}

const char* GetHost()
{
    static char hostname[HOST_NAME_MAX];
    gethostname(hostname,sizeof(hostname));
    return hostname==NULL?"None":hostname;
}

const char* GetPwd()
{
    const char* pwd=getenv("PWD");
    return pwd==NULL?"None":pwd;
}

void MakeCommandPrompt(char cmd_prompt[],int size)
{
    snprintf(cmd_prompt,size,FORMAT,GetUserName(),GetHost(),GetPwd());
}

void PrintCommandPrompt()
{
    char prompt[COMMAND_PROMAT_SIZE_MAX];
    MakeCommandPrompt(prompt,sizeof(prompt));
    printf("%s",prompt);
    fflush(stdout);//刷新
}

bool GetUserCommand(char* user_cmd,int size)
{
    char* c=fgets(user_cmd,size,stdin);
    if(c==nullptr)
        return false;
    user_cmd[strlen(user_cmd)-1]=0;
    return true;
}

bool CommandParse(char* commandline)
{
    #define SEP " "//还能这样定义？
    argc=0;
    argv[argc++]=strtok(commandline,SEP);
    while(argv[argc++]=strtok(nullptr,SEP));//若为nullptr 其会在上一次切的地方继续切割 且argv最后一个被设置为nullptr
    argc--;
    return true;
}

void PrintArgv()
{
    for(int i=0;argv[i];i++)
    {
        printf("%s\n",argv[i]);
    }
}

int Execute()
{
    pid_t id=fork();
    if(id==0)
    {
        execvp(argv[0],argv);
        return 1;
    }
    pid_t rid=waitpid(id,nullptr,0);
    (void)rid;
    return 0;
}

int main()
{
    while(true)
    {
        //打印命令行提示符
        PrintCommandPrompt();
        //获取用户命令
        char user_cmd[COMMAND_SIZE_MAX];
        if(!GetUserCommand(user_cmd,sizeof(user_cmd)))
        {
            continue;
        }
        //命令行分析
        CommandParse(user_cmd);
        //PrintArgv();

        //执行命令
        Execute();
    }
    return 0;
}
