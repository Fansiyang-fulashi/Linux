#include"TcpServer.hpp"
#include"log.hpp"
#include"cal.hpp"
#include"Task.hpp"
#include"Daemon.hpp"

int main(int argc,char*argv[])
{
	if(argc!=2)
	{
		LOG(LogLevel::FATAL)<<"使用错误";
		exit(ERROR::USE_ERROR);
	}
	LOG(LogLevel::INFO)<<"服务器启动";
	Daemon();

	std::unique_ptr<Cal> c=std::make_unique<Cal>();

	std::unique_ptr<Protocol> p=std::make_unique<Protocol>([&c](Request& req)->Response{
		return c->Execute(req);
	});

	std::unique_ptr<TcpServer> t=std::make_unique<TcpServer>(std::stoi(argv[1]),[&p](std::shared_ptr<Socket> sock,InetAddr client){
		p->GetRequest(sock,client);
	});
	t->Start();
	return 0;
}