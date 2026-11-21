#include"Socket.hpp"
#include"cal.hpp"
#include"Task.hpp"

void GetData(int* x,int* y,char* oper)
{
	std::cout<<"请输入x:";
	std::cin>>*x;
	std::cout<<"请输入y:";
	std::cin>>*y;
	std::cout<<"请输入oper:";
	std::cin>>*oper;
}

int main(int argc,char*argv[])
{
	if(argc!=3)
	{
		LOG(LogLevel::FATAL)<<"使用错误";
		exit(ERROR::USE_ERROR);
	}
	std::unique_ptr<Socket> client=std::make_unique<TcpSocket>();
	client->BuildClientTcpSocket();
	int n=client->Connect(argv[1],std::stoi(argv[2]));
	if(n!=0)
	{
		LOG(LogLevel::FATAL)<<"连接错误";
		exit(ERROR::CONNECT_ERROR);
	}
	std::unique_ptr<Protocol> p=std::make_unique<Protocol>();
	while(true)
	{
		int x,y;
		char oper;
		GetData(&x,&y,&oper);
		std::string send_package=p->MakeRequset(x,y,oper);
		int n=client->Send(send_package);
		if(n<0)
		{
			LOG(LogLevel::ERROR)<<"发送错误";
			continue;
		}
		LOG(LogLevel::ERROR)<<"发送成功";
		Response resp;
		bool ok=p->GetResponse(client,&resp);
		if(!ok)
			continue;
		LOG(LogLevel::ERROR)<<"成功";
		Cal().Execute(resp);
	}
	client->Close();
}