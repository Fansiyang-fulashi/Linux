#include<iostream>
#include<functional>
#include<unistd.h>

using func_t =std::function<void()>;

void Download()
{
	std::cout<<"下载一个任务"<<std::endl;
}