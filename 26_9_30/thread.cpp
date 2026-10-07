#include"comm.hpp"

void* ThreadRun(void* argv)
{
	std::string p=(char*)argv;
	while(true)
	{
		std::cout<<"p"<<std::endl;
		sleep(1);
	}
}

int main()
{
	pthread_t tid;
	char* a="ffff";
	pthread_create(&tid,nullptr,ThreadRun,a);
	while(true)
	{
		std::cout<<"llll"<<std::endl;
		sleep(2);
	}
	return 0;
}