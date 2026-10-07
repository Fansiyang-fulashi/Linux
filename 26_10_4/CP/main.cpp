#include"BlockQueue.hpp"
#include"Task.hpp"

void* consumer(void* argv)
{
	BlockQueue<int>* bq=static_cast<BlockQueue<int>*>(argv);
	while(true)
	{
		sleep(1);
		std::cout<<"消费一个数据:"<<bq->Pop()<<std::endl;
	}
}

void* productor(void* argv)
{
	int data=1;
	BlockQueue<int>* bq=static_cast<BlockQueue<int>*>(argv);
	while(true)
	{
		sleep(1);
		bq->Push(data);
		std::cout<<"生产一个数据"<<std::endl;
		data++;
	}
}

int main()
{
	BlockQueue<int>* bq=new BlockQueue<int>();
	pthread_t c[2],p[3];
	pthread_create(c,nullptr,consumer,bq);
	//pthread_create(c+1,nullptr,consumer,bq);

	pthread_create(p,nullptr,productor,bq);
	// pthread_create(p+1,nullptr,productor,bq);
	// pthread_create(p+2,nullptr,productor,bq);
	
	pthread_join(c[0],nullptr);
	// pthread_join(c[1],nullptr);
	
	pthread_join(p[0],nullptr);
	// pthread_join(p[1],nullptr);
	// pthread_join(p[2],nullptr);
	return 0;
}