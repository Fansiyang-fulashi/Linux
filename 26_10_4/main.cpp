#include"comm.hpp"

pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t con=PTHREAD_COND_INITIALIZER;

#define NUM 5

int ticket=100;

void* threadrun(void* argv)
{
	std::string name=static_cast<char*>(argv);
	while(true)
	{
		pthread_mutex_lock(&lock);
		pthread_cond_wait(&con,&lock);
		std::cout<<name<<':'<<ticket<<std::endl;
		ticket++;
		pthread_mutex_unlock(&lock);
	}
}

int main()
{
	std::vector<pthread_t> pmts;
	for(int i=0;i<NUM;i++)
	{
		pthread_t id;
		char* name=new char[64];
		snprintf(name,64,"thread-%d",i);
		int n=pthread_create(&id,nullptr,threadrun,name);
		pmts.push_back(id);
		sleep(1);
	}

	while(1)
	{
		pthread_cond_signal(&con);
		sleep(1);
	}

	for(auto&x:pmts)
	{
		pthread_join(x,nullptr);
	}

}