#include"TcpServer.hpp"
#include"dic.hpp"
#include"command.hpp"

const std::string func(const std::string& word)
{
	Command cmd;
	return cmd.Execute(word);
}

int main(int argc,char*argv[])
{
	if(argc!=2)
	{
		LOG(LogLevel::ERROR)<<"输入错误";
		exit(ERROR::USE_ERROR);
	}

	std::unique_ptr<TcpServer> s=std::make_unique<TcpServer>(std::stoi(argv[1]),func);

	s->Init();

	s->Start();

}