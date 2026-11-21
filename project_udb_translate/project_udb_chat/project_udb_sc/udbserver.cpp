#include<iostream>
#include"log.hpp"
#include<memory>
#include"udbserver.hpp"
#include"dic.hpp"
#include"InetAddr.hpp"

std::string del(const std::string& s)
{
	std::cout<<s;
	return s;
}

int main(int argc,char*argv[])
{
	Enable_Console_Log_Stategy();
	if(argc!=2)
	{
		LOG(LogLevel::ERROR)<<"使用错误";
		return 1;
	}

	uint16_t port=std::stoi(argv[1]);

	Dic dic;
	dic.Load();

	std::unique_ptr<UdbServer> server=std::make_unique<UdbServer>(port,[&dic](std::string chinese,InetAddr& inetaddr)->std::string{
		return dic.Find(chinese,inetaddr);
	}
	);

	server->Init();
	server->Start();
}