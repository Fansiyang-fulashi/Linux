#pragma once
#include<iostream>
#include<functional>
#include<vector>
#include<ctime>

void PrintLog()
{
    std::cout<<"print log..."<<std::endl;
}

void Download()
{
    std::cout<<"download..."<<std::endl;
}

void Accept()
{
    std::cout<<"accept..."<<std::endl;
}

class taskmanager
{
public:
    taskmanager()
    {
        srand(time(nullptr));
    }

    ~taskmanager()
    {}

    void Push(std::function<void()> f)
    {
        _taskv.push_back(f);
    }

    size_t ReturnTaskCode()
    {
        return rand()%_taskv.size();
    }

    void Execute(size_t i)
    {
        if(i<_taskv.size()&&i>=0)
        _taskv[i]();
    }

private:
    std::vector<std::function<void()>> _taskv;
};

