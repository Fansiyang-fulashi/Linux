// #include"comm.hpp"

// int ticket=1000;

// class mutex_data
// {
// public:
// 	mutex_data(pthread_mutex_t* pmt,std::string name)
// 	:_pmt(pmt)
// 	,_name(name)
// 	{}

// 	~mutex_data()
// 	{}

// 	pthread_mutex_t* _pmt;
// 	std::string _name;
// };

// void* func(void* argv)
// {
// 	mutex_data* m=static_cast<mutex_data*>(argv);
// 	while(true)
// 	{
// 		pthread_mutex_lock((m->_pmt));
// 		if(ticket>0)
// 		{
// 			usleep(1000);
// 			printf("%s:%d\n",m->_name.c_str(),ticket);
// 			ticket--;
// 			pthread_mutex_unlock((m->_pmt));
// 		}
// 		else
// 		{
// 			pthread_mutex_unlock((m->_pmt));
// 			break;
// 		}
// 	}
// 	return nullptr;
// }

// int main()
// {
// 	pthread_mutex_t lock;
// 	pthread_mutex_init(&lock,nullptr);
// 	pthread_t p1,p2,p3,p4;
// 	mutex_data* m1=new mutex_data(&lock,"lock1");
// 	mutex_data* m2=new mutex_data(&lock,"lock2");
// 	mutex_data* m3=new mutex_data(&lock,"lock3");
// 	mutex_data* m4=new mutex_data(&lock,"lock4");
// 	pthread_create(&p1,nullptr,func,m1);
// 	pthread_create(&p2,nullptr,func,m2);
// 	pthread_create(&p3,nullptr,func,m3);
// 	pthread_create(&p4,nullptr,func,m4);

// 	pthread_join(p1,nullptr);
// 	pthread_join(p2,nullptr);
// 	pthread_join(p3,nullptr);
// 	pthread_join(p4,nullptr);

// 	pthread_mutex_destroy(&lock);
// 	std::cout<<"end"<<std::endl;
// }


#include"mutex.hpp"

int main()
{
	
}