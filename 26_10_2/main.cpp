#include"thread.hpp"

void func(int a)
{
	int cnt=5;
	while(cnt--)
	{
		std::cout<<"运行中。。。。"<<std::endl;
		sleep(1);
	}
}

int main()
{
	thread<int> it(std::string("fjenf"),func,12);
	it.Start();
	sleep(8);
	it.Stop();
	it.Join();
}