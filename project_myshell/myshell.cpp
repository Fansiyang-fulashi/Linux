#include<stdio.h>
//上一个路径
char* PREVPATH=getenv("PWD");

//路径缓冲区
#define CWD_BUF_MAX 1024
char buf[CWD_BUF_MAX];
char prev_buf[CWD_BUF_MAX];

//环境变量缓冲
#define BUF_MAX 1024
char pwd_buf[BUF_MAX];

//最近一次进程退出码
int lastexit=0;

//环境变量表
#define ENV_MAX 100
char* envs[ENV_MAX];
int envc=0;

//别名表
#define ALIAS_SIZE_MAX 100
std::unordered_map<std::string,std::string> alias_table;

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
    //const char* pwd=getenv("PWD");
    const char* pwd=getcwd(buf,sizeof(buf));
    if(pwd!=NULL)
    {
        snprintf(pwd_buf,sizeof(pwd_buf),"PWD=%s",pwd);
        putenv(pwd_buf);
    }
    return pwd==NULL?"None":pwd;
}

const char* GetHome()
{
    const char* home=getenv("HOME");
    return home==NULL?"":home;
}

void InitEnv()
{
    extern char** environ;
    memset(envs,0,sizeof(envs));
    for(int i=0;environ[i];i++)
    {
        envs[i]=(char*)malloc(strlen(environ[i]));
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
    return argc>0;
}

void PrintArgv()
{
    for(int i=0;argv[i];i++)
    {
        printf("%s\n",argv[i]);
    }
}

void Cd()
{
    if(argc==1)
  {
      std::string home=GetHome();
      if(home.empty())
          return;
      PREVPATH=getcwd(prev_buf,sizeof(prev_buf));
      chdir(home.c_str());
  }
  else
  {
      std::string where=argv[1];
      if(where=="-")
      {
          if(PREVPATH!=NULL)
          chdir(PREVPATH);
      }
      else if(where=="~")
      {
          PREVPATH=getcwd(prev_buf,sizeof(prev_buf));
          where=GetHome();
          if(where.empty())
              return;
          chdir(where.c_str());
      }
      else
      {
          PREVPATH=getcwd(prev_buf,sizeof(prev_buf));
          chdir(where.c_str());
      }
  }
}

void Export()
{
#define ENVBUF_SIZE_MAX 1024
    if(argc==2)
    {
        char envbuf[ENVBUF_SIZE_MAX];
        memset(envbuf,sizeof(envbuf),0);
        auto has_env=[&]{
            for(int i=0;envs[i];i++)
            {   
                envbuf=strtok(envs[i],"=");
                if(strcmp(argv[1],envbuf)==0)
                    return i;
            }
            return -1;
        };
        int exist=has_env();
        if(exist==-1)
        {
            envs[envc]=(char*)malloc(sizeof(argv[1]));
            strcpy(envs[envc],argv[1]);
            envc++;
            envs[envc]=0;
        }
        else
        {
            strcpy(envs[exist],argv[1]);
        }
    }
}

void Alias()
{
    if(argc==2)
    {
        char* key=strtok(argv[1],"=");
        for(int i=0;;i++)
        {
            if(argv[1][i]=='=')
            {
                char* value=argv[1]+i+1;
            }
        }
        alias_table[key]=value;
        return true;
    }
}

void Echo()
{
    if(argc==2)
    {
        std::string opt=argv[1];
        if(opt=="$?")
        {
            std::cout<<lastexit<<std::endl;
            lastexit=0;
            return;
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
        Echo();
        return true;
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

int Execute()
{
    pid_t id = fork();
    if(id == 0)
    {
        //child
        execvp(argv[0], argv);
        exit(1);
    }
    int status = 0;
    // father
    pid_t rid = waitpid(id, &status, 0);
    if(rid > 0)
    {
        lastexit = WEXITSTATUS(status);
    }
    return 0;
}

void CleanUp()
{
    for(int i=0;envs[i];i++)
    {
        free(envs[i]);
    }
}

bool CheckAlias()
{
    if(alias_table.count==1)
    {
        char commandline[COMMAND_SIZE_MAX];
        if(!GetUserCommand(commandline, sizeof(commandline)))
            return true;
        if(!CommandParse(commandline))
            return true;
        if(CheckAndExecBuiltin())
            return true;
        Execute();
        return true;
    }
    return false;
}

int main()
{
    //初始化构建环境变量表
    InitEnv();

    while(true)
    {
        // 1. 输出命令行提示符
        PrintCommandPrompt();

        // 2. 获取用户输入的命令
        char commandline[COMMAND_SIZE_MAX];
        if(!GetUserCommand(commandline, sizeof(commandline)))
            continue;

        // 3. 命令行分析 "ls -a -l" -> "ls" "-a" "-l"
        if(!CommandParse(commandline))
            continue;
        //PrintArgv();

        // 检测别名
        CheckAlias();
        // 4. 检测并处理内键命令
        if(CheckAndExecBuiltin())
            continue;

        // 5. 执行命令
        Execute();
    }
    CleanUp();
    return 0;
}

