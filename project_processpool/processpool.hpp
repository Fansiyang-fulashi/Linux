#ifndef __PROCESS_POOL_HPP
#define __PROCESS_POOL_HPP

#include<iostream>
#include<vector>
#include<unistd.h>
#include"task.hpp"
#include<sys/wait.h>

class channel
{
public:
    channel(int fid,pid_t pid,std::string name)
    :_fid(fid)
    ,_pid(pid)
    ,_name(name)
    {
    }

    ~channel()
    {}

    int fid()
    {
        return _fid;
    }

    pid_t pid()
    {
        return _pid;
    }

    std::string name()
    {
        return _name;
    }

    void Send(size_t code)
    {
        (void)write(_fid,&code,sizeof(code));
        std::cout<<_fid<<":send "<<code<<std::endl;
    }

private:
    int _fid;
    pid_t _pid;
    std::string _name;
};

class channel_manager
{
public:
    channel_manager()
    :_now(0)
    {
    }

    ~channel_manager()
    {}

    void Add(int fid,pid_t pid)
    {
        std::string name="channel"+std::to_string(fid)+"-"+std::to_string(pid);
        _chv.emplace_back(fid,pid,name);
    }

    void WatchChannel()
    {
        for(auto&ch:_chv)
        {
            std::cout<<ch.fid()<<" "<<ch.pid()<<" "<<ch.name()<<std::endl;
        }
    }

    channel& ChooseProcess()
    {
        size_t i=_now;
        _now++;
        _now%=_chv.size();
        std::cout<<"choose "<<i<<std::endl;
        return _chv[i];
    }

    void Wait()
    {
        for(auto& ch:_chv)
        {
            waitpid(ch.pid(),nullptr,0);
        }
    }

    void Close()
    {
        for(auto& ch:_chv)
        {
            close(ch.fid());
        }
    }
private:
    std::vector<channel> _chv;
    size_t _now;
};

class processpool
{
public:
    processpool(size_t num)
    :_process_num(num)
    {
        _tm.Push(PrintLog);
        _tm.Push(Download);
        _tm.Push(Accept);
    }

    ~processpool()
    {
    }

    bool Init()
    {
        for(size_t i=0;i<_process_num;i++)
        {
            int pipefd[2]={0};
            int piper=pipe(pipefd);
            fflush(stdout);
            pid_t pid=fork();
            if(pid<0)
            {
                std::cerr<<"fork wrong"<<std::endl;
                return false;
            }
            else if(pid==0)
            {
                close(pipefd[1]);
                Work(pipefd[0]);
                close(pipefd[0]);
                exit(0);
            }
            else 
            {
                close(pipefd[0]);
                _cm.Add(pipefd[1],pid);
            }
        }
        return true;
    }

    void Run()
    {
        size_t code=_tm.ReturnTaskCode();
        channel& ch=_cm.ChooseProcess();
        ch.Send(code);
    }

    void WaitAllProcess()
    {
        _cm.Wait();
    }

    void CloseAllFile()
    {
        _cm.Close();
    }
private:

    void Work(int fid)
    {
        while(true)
        {
            size_t n=0;
            int readr=read(fid,&n,sizeof(n));
            if(readr<0)
            {
                std::cerr<<fid<<"read wrong"<<std::endl;
                break;
            }
            else if(readr==0)
            {
                std::cout<<fid<<"read end"<<std::endl;
                break;
            }
            else 
            {
                if(readr!=sizeof(n))
                continue;
                _tm.Execute(n);
            }
        }
    }

    channel_manager _cm;
    size_t _process_num;
    taskmanager _tm;
};

#endif