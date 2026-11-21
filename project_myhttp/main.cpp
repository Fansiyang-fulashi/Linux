#include"Http.hpp"

void login(std::string url,HttpResponse*)
{
	std::cout<<"登录成功"<<std::endl;
	std::cout<<"登录成功"<<std::endl;
	std::cout<<"登录成功"<<std::endl;
	std::cout<<"登录成功"<<std::endl;
	std::cout<<"登录成功"<<std::endl;
}

int main(int argc,char*argv[])
{
	if(argc!=2)
	{
		LOG(LogLevel::ERROR)<<"使用错误";
		exit(ERROR::USE_ERROR);
	}
	std::unique_ptr<Http> hp=std::make_unique<Http>(std::stoi(argv[1]));
	hp->Regist("login",login);
	hp->Start();
	
	while(1)
	{}
}