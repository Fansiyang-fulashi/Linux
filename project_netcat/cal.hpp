#pragma once

#include<iostream>
#include <sys/types.h>         
#include <sys/socket.h>
#include <strings.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<functional>
#include"log.hpp"
#include"Socket.hpp"
#include<jsoncpp/json/json.h>

enum CODE_KEY
{
	DEFAULT=-1,
	TRUE,
	UNDINE_CAL,
	DIVISION_BY_ZERO,
	MODUIO_BY_ZERO,
};

const std::string SEP="\r\n";

class Request
{
public:
	Request()
	{}

	Request(int x,int y,char oper)
	:_x(x)
	,_y(y)
	,_oper(oper)
	{}

	std::string Serlalize()
	{
		Json::Value root;
		root["x"]=_x;
		root["y"]=_y;
		root["oper"]=_oper;
		return root.toStyledString();
	}

	void Deserlalize(const std::string& s)
	{
		Json::Value root;
		Json::Reader read;
		bool ok=read.parse(s,root);
		if(ok==true)
		{
			_x=root["x"].asInt();
			_y=root["y"].asInt();
			_oper=root["oper"].asInt();
		}
	}

	int X()
	{
		return _x;
	}

	int Y()
	{
		return _y;
	}

	int Oper()
	{
		return _oper;
	}

private:
	int _x;
	int _y;
	char _oper;
};

class Response
{
public:
	Response()
	:_code(CODE_KEY::DEFAULT)
	{}

	std::string Serlalize()
	{
		Json::Value root;
		root["result"]=_result;
		root["_code"]=_code;
		return root.toStyledString();
	}

	void Deserlalize(const std::string& s)
	{
		Json::Value root;
		Json::Reader read;
		bool ok = read.parse(s, root);
		if (ok == true)
		{
			_result=root["result"].asInt();
			_code=root["code"].asInt();
		}
	}

	int& SetResult()
	{
		return _result;
	}

	int& SetCode()
	{
		return _code;
	}
private:
	int _result;
	int _code;
};

using func_t =std::function<Response(Request&)>;

class Protocol
{
public:
	Protocol(func_t fun)
	:_fun(fun)
	{}

	Protocol()
	{}
	
	std::string Encode(const std::string& s)
	{
		int len=s.size();
		return std::to_string(len)+SEP+s+SEP;
	}

	bool Decode(const std::string& s,std::string* package)
	{
		ssize_t pos=s.find(SEP);
		if(pos==std::string::npos)
		{
			return false;
		}
		std::string len_str=s.substr(0,pos);
		int len=std::stoi(len_str);
		int target_size=2*SEP.size()+len+len_str.size();
		if(s.size()<target_size)
		{
			return false;
		}
		*package=s.substr(pos+SEP.size(),len);
		return true;
	}

	void GetRequest(std::shared_ptr<Socket>& socket,InetAddr& client)
	{
		std::string buf;
		while (true)
		{
			int n = socket->Read(&buf);
			if(n>0)
			{
				LOG(LogLevel::DEBUG)<<"读取成功";
				std::string package;
				bool res=Decode(buf,&package);
				if(!res)
				{
					LOG(LogLevel::ERROR)<<"报文不正确";
					continue;
				}
				Request req;
				req.Deserlalize(package);
				Response resq=_fun(req);
				std::string json_string=resq.Serlalize();
				std::string send_package=Encode(json_string);
				socket->Send(send_package);
				buf.clear();
			}
			else if(n==0)
			{
				LOG(LogLevel::INFO)<<"客户端退出";
				break;
			}
			else 
			{
				LOG(LogLevel::WARNING)<<"读取错误";
				break;
			}
		}
	}

	std::string MakeRequset(int x,int y,char oper)
	{
		Request req(x,y,oper);
		std::string package=req.Serlalize();
		return Encode(package);
	}

	Response ParsePackage(std::string& buf)
	{
		std::string package;
		bool res = Decode(buf, &package);
		if (!res)
		{
			LOG(LogLevel::ERROR)<<"报文不正确";
			return Response();
		}
		Response resp;
		resp.Deserlalize(package);
		return resp;
	}

	bool GetResponse(std::unique_ptr<Socket>& client,Response* resp)
	{
		std::string res;
		while(true)
		{
			int n = client->Read(&res);
			if(n==0)
			{
				LOG(LogLevel::INFO)<<"服务端退出";
				return false;
			}
			else if(n<0)
			{
				LOG(LogLevel::ERROR)<<"读取错误";
				return false;
			}
			LOG(LogLevel::INFO)<<"读取正确";
			std::string package;
			bool ok = Decode(res, &package);
			if (!ok)
			{
				LOG(LogLevel::ERROR) << "报文不正确";
				continue;
			}
			Response response;
			response.Deserlalize(package);
			*resp = response;
			break;
		}
		return true;
	}

	~Protocol()
	{}
private:
	func_t _fun;
};