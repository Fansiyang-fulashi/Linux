#include"TcpServer.hpp"
#include"log.hpp"
#include"comm.hpp"
#include"Listener.hpp"
#include"Task.hpp"
#include"cal.hpp"

int main(int argc,char*argv[])
{
	if(argc!=2)
	{
		LOG(LogLevel::FATAL)<<"used error";
		exit(ERROR::USE_ERROR);
	}
	std::shared_ptr<Cal> task=std::make_shared<Cal>();

	std::shared_ptr<Protocol> protocol=std::make_shared<Protocol>([task](Request& resq){
		return task->Execute(resq);
	});

	std::shared_ptr<Listener> listen=std::make_shared<Listener>(std::stoi(argv[1]));

	listen->RegistHandler([&protocol](std::string& s){
		std::string package;
		std::string res_str;
		while(true)
		{
			if(!protocol->Decode(s,&package))
				break;
			res_str+=protocol->GetRequest(package);
		}
		return protocol->GetRequest(package);
	});

	std::unique_ptr<TcpServer> s=std::make_unique<TcpServer>();

	s->AddConnection(listen);

	s->Start();
	
}