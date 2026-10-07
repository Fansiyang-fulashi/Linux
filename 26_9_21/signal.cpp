#include"comm.hpp"

void fun(int a)
{
	printf("标号为：%d",a);
}

int main()
{
	signal(SIGINT,fun);
	while(true)
	{
		printf("okoo\n");
		sleep(2);
	}
	return 0;
}