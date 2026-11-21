#include<iostream>
#include"log.hpp"
#include<memory>
#include<functional>
#include"udbserver.hpp"
#include"InetAddr.hpp"
#include"route.hpp"
#include"threadpool.hpp"

using task_t =std::function<void()>;

int main(int argc,char*argv[])
{
	Enable_Console_Log_Stategy();
	if(argc!=2)
	{
		LOG(LogLevel::ERROR)<<"使用错误";
		return 1;
	}

	uint16_t port=std::stoi(argv[1]);

	Route route;

	auto tp=ThreadPool<task_t>::GetInstance();

	std::unique_ptr<UdbServer> server=std::make_unique<UdbServer>(port,[&route,&tp](int socket,std::string message,InetAddr& inetaddr){
		auto t=std::bind(Route::MessageSend,route,socket,message,inetaddr);
		tp->Push(t);
	}
	);

	server->Init();
	server->Start();
}