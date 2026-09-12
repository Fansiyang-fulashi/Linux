#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <unordered_map>
#include <sys/stat.h>
#include <fcntl.h>


#define COMMAND_SIZE 1024

#define CWD_BUF_SIZE 1024
char cwd_buf[CWD_BUF_SIZE];

#define CWDENV_SIZE 1024
char cwdenv[CWDENV_SIZE];

#define HOST_NAME_SIZE 256

const char* FORMAT="%s@%s:%s$ ";

#define COMMANDLINE_SIZE 1024

#define MAX_ARGV_SIZE 128
char* argv[MAX_ARGV_SIZE];
int argc=0;

int lastcode=0;

char* PREVPATH=getenv("PWD");
#define PREVPATH_BUF_SIZE 256
char prevpath_buf[PREVPATH_BUF_SIZE];

#define ENVS_SIZE 1024
char* envs[ENVS_SIZE];
int envc=0;

#define ENV_BUF_SIZE 256

std::unordered_map<std::string,std::string> alias_table;

#define NO_REDIR 0
#define INPUT_REDIR 1
#define OUTPUT_REDIR 2
#define APPEND_REDIR 3
int redir=0;
#define FILE_NAME_SIZE 256
std::string file_name;

const char* GetUserName()
{
    const char* name=getenv("USER");
    return name==NULL?"None":name;
}

const char* GetHostName()
{
    static char hostname[HOST_NAME_SIZE];
    gethostname(hostname,sizeof(hostname));
    return hostname==NULL?"None":hostname;
}

const char* GetPwd()
{
    const char* pwd=getcwd(cwd_buf,sizeof(cwd_buf));
    if(pwd!=NULL)
    {
        snprintf(cwdenv,sizeof(cwdenv),"PWD=%s",cwd_buf);
        putenv(cwdenv);
    }
    return pwd==NULL?"None":pwd;
}

const char* GetHome()
{
    const char* home=getenv("HOME");
    return home==NULL?"None":home;
}

void MakeCommandPrompt(char prompt[],int size)
{
    snprintf(prompt,size,FORMAT,GetUserName(),GetHostName(),GetPwd());
}

void PrintCommandPrompt()
{
    char prompt[COMMAND_SIZE];
    MakeCommandPrompt(prompt,sizeof(prompt));
    printf("%s",prompt);
    fflush(stdout);
}

bool GetCommandline(char* out,int size)
{
    char* comline=fgets(out,size,stdin);
    if(comline==NULL) return false;
    out[strlen(out)-1]=0;//
    if(strlen(out)==0) return false;
    return true;
}

bool CommandlineParse(char* out)
{
#define SEP " "
    argc=0;
    argv[argc++]=strtok(out,SEP);
    while((bool)(argv[argc++]=strtok(NULL,SEP)));
    argc--;
    return argc>0?true:false;
}

void PrintArgv()
{
    for(int i=0;i<argc;i++)
    {
        printf("%s\n",argv[i]);
    }
}

int Execute()
{
    pid_t id=fork();
    if(id==0)
    {
        int fd=-1;
        if(redir==INPUT_REDIR)
        {
            fd=open(file_name.c_str(),O_RDONLY);
            if(fd<0) exit(1);
            dup2(fd,0);
            close(fd);
        }
        else if(redir==OUTPUT_REDIR)
        {
            fd=open(file_name.c_str(),O_CREAT|O_WRONLY|O_TRUNC,0666);
            if(fd<0) exit(1);
            dup2(fd,1);
            close(fd);
        }
        else if(redir==APPEND_REDIR)
        {
            fd=open(file_name.c_str(),O_CREAT|O_WRONLY|O_APPEND,0666);
            if(fd<0) exit(1);
            dup2(fd,1);
            close(fd);
        }
        execvp(argv[0],argv);
        exit(1);
    }
    int status=0;
    pid_t rid=waitpid(id,&status,0);
    if(rid>0)
    {
        lastcode=WEXITSTATUS(status);
    }
    return 0;
}

bool Cd()
{
    if(argc==1)
    {
        std::string home=GetHome();
        if(home=="None") return true;
        PREVPATH=getcwd(prevpath_buf,sizeof(prevpath_buf));
        chdir(home.c_str());
    }
    else 
    {
        std::string where=argv[1];
        if(where=="-")
        {
            if(PREVPATH==NULL)
                return true;
            chdir(PREVPATH);
        }
        else if(where=="~")
        {
            std::string home=GetHome();
            if(home=="None") return true;
            PREVPATH=getcwd(prevpath_buf,sizeof(prevpath_buf));
            chdir(home.c_str());
        }
        else
        {
            PREVPATH=getcwd(prevpath_buf,sizeof(prevpath_buf));
            chdir(where.c_str());
        }
    }
    return true;
}

void Echo()
{
   if(argc==2)
   {
        std::string opt=argv[1];
        if(opt=="$?")
        {
            std::cout<<lastcode<<std::endl;
        }
        else if(opt[0]=='$')
        {
            std::string env_name=opt.substr(1);
            const char* env=getenv(env_name.c_str());
            if(env)
                std::cout<<env<<std::endl;
        }
        else
        {
            std::cout<<opt<<std::endl;
        }
   }
   return;
}

void Export()
{
    if(argc==2)
    {
        char* tmp_env;
        auto has_env=[&]{
            for(int i=0;envs[i];i++)
            {
               tmp_env=strchr(envs[i],'=');
               if(tmp_env==NULL) continue;
               if(strcmp(tmp_env,argv[1])==0)
                   return i;
            }
            return -1;
        };
        int exist=has_env();
        if(exist==-1)
        {
            envs[envc]=(char*)malloc(strlen(argv[1]));
            strcpy(envs[envc],argv[1]);
            envc++;
            envs[envc]=NULL;
        }
        else
        {
            strcpy(envs[exist],argv[1]);
        }
    }
    return;
}

void Alias()
{
    if(argc==1)
    {
        for(auto& x:alias_table)
        {
            std::cout<<x.first<<'='<<x.second<<std::endl;
        }
    }
    else if(argc>=2)
    {
        std::string s=argv[1];
        for(int i=2;i<argc;i++)
        {
            s+=" ";
            s+=argv[i];
        }
        size_t pos=s.find('=');
        if(pos==s.npos)
            return;
        std::string key=s.substr(0,pos);
        std::string value=s.substr(pos+1);
        alias_table[key]=value;
    }
    return;
}

bool CheckAndExecBuiltin()
{
    std::string cmd=argv[0];
    if(cmd=="cd")
    {
        Cd();
        return true;
    }
    else if(cmd=="echo")
    {
        //if(redir==NO_REDIR)
        //{
            Echo();
            return true;
        //}
    }
    else if(cmd=="export")
    {
        Export();
        return true;
    }
    else if(cmd=="alias")
    {
        Alias();
        return true;
    }
    return false;
}

void InitEnv()
{
    extern char** environ;
    memset(envs,0,sizeof(envs));
    for(int i=0;environ[i];i++)
    {
        envs[i]=(char*)malloc(strlen(environ[i])+ENV_BUF_SIZE);
        strcpy(envs[i],environ[i]);
        envc++;
    }
    envs[envc]=NULL;
    for(int i=0;envs[i];i++)
    {
        putenv(envs[i]);
    }
    environ=envs;
}

bool CheckAlias(char* out)
{
    std::string key=out;
    if(alias_table.count(key)==0)
    {
        return false;
    }
    std::string cmd=alias_table[key];
    if(!CommandlineParse((char*)cmd.c_str()))
        return true;
    if(CheckAndExecBuiltin())
        return true;
    Execute();
    return true;
}

void CheckRedir(char* out)
{
    redir=NO_REDIR;
    std::string s=out;
    if(s.find(">>")!=s.npos)
    {
        redir=OUTPUT_REDIR;
        size_t pos=s.find(">>");
        out[pos]=0;
        size_t fpos=pos+2;
        for(;fpos<s.size();fpos++)
        {
            if(s[fpos]!=' ')
                break;
        }
        file_name=s.substr(fpos);
    }
    else if(s.find('>')!=s.npos)
    {
        redir=APPEND_REDIR;
        size_t pos=s.find(">");
        out[pos]=0;
        size_t fpos=pos+1;
        for(;fpos<s.size();fpos++)
        {
            if(s[fpos]!=' ')
                break;
        }
        file_name=s.substr(fpos);
    }
    else if(s.find('<')!=s.npos)
    {
        redir=INPUT_REDIR;
        size_t pos=s.find("<");
        out[pos]=0;
        size_t fpos=pos+1;
        for(;fpos<s.size();fpos++)
        {
            if(s[fpos]!=' ')
                break;
        }
        file_name=s.substr(fpos);
    }
}

int main()
{
    InitEnv();
    while(true)
    {
        PrintCommandPrompt();    
        char commandline[COMMANDLINE_SIZE];
        if(!GetCommandline(commandline,sizeof(commandline)))
                continue;
        CheckRedir(commandline);
        if(!CommandlineParse(commandline))
                continue;
        if(CheckAndExecBuiltin())
                continue;
        if(CheckAlias(commandline))
                continue;
        Execute();
    }
}
