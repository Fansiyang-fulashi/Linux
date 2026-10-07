#include"comm.hpp"
#define FUNNUM 3
using fun=std::function<void()>;

std::vector<fun> funs;

//#######################################################
void DetermineTask()
{
	std::cout<<"正在确定下载任务，请稍候。。。"<<std::endl;
}

void Download()
{
	std::cout<<"正在下载，等待下载完成。。。"<<std::endl;
}

void Finish()
{
	std::cout<<"下载完成"<<std::endl;
}
//#############################################################

void Regist()
{
	funs.push_back(DetermineTask);
	funs.push_back(Download);
	funs.push_back(Finish);
}

void Execute(int)
{
	std::cout<<"####################################"<<std::endl;
	for(auto& fun:funs)
	{
		fun();
	}
	std::cout<<"####################################"<<std::endl;
	alarm(1);
}

int main()
{
	Regist();
	signal(SIGALRM,Execute);
	alarm(1);
	while(true)
	{	
		;
	}
	return 0;
}