#include"comm.hpp"

void PrintPending(sigset_t p)
{
	std::cout<<"i am process:";
	for(int signo=31;signo>=1;signo--)
	{
		if(sigismember(&p,signo))
		{
			std::cout<<"1";
		}
		else
		{
			std::cout<<"0";
		}
	}
	std::cout<<std::endl;
}

int main()
{
	sigset_t block;
	sigset_t oblock;
	sigemptyset(&block);
	sigemptyset(&oblock);
	sigaddset(&block,SIGINT);
	int n=sigprocmask(SIG_SETMASK,&block,&oblock);
	while(true)
	{
		sigset_t pending;
		int m=sigpending(&pending);
		PrintPending(pending);
		sleep(1);
	}
}