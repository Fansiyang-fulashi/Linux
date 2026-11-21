#pragma once

#include<iostream>
#include<sstream>
#include <sys/types.h>         
#include <sys/socket.h>
#include <strings.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <sys/wait.h>
#include <functional>
#include <sys/types.h>
#include <unistd.h>
#include<memory>
#include<unordered_map>
#include"log.hpp"
#include"comm.hpp"
#include"TcpServer.hpp"
#include"tool.hpp"
#include"InetAddr.hpp"

const std::string SPACE_SEP=" ";
const std::string DAULT_BLANK_LINE="\r\n";
const std::string KV_SEP=": ";

const std::string FILE_PATH="./index";

class HttpResponse;

using http_func=std::function<void(std::string&,HttpResponse*)>;

class HttpRequest
{
public:
	HttpRequest()
	:_blankline(DAULT_BLANK_LINE)
	{}

	std::string Serlalize(HttpRequest& req)
	{
		std::string head_str=req._method+SPACE_SEP+req._url+SPACE_SEP+_version+DAULT_BLANK_LINE;
		std::string kv_str;
		for(auto& x:_kv)
		{
			std::string line=x.first+KV_SEP+x.second+DAULT_BLANK_LINE;
			kv_str+=line;
		}
		return head_str+kv_str+_blankline+_text;
	}

	bool DeSerlalize(std::string& _str)
	{
		std::string line;
		if(!Tool::GetLine(_str,DAULT_BLANK_LINE,&line))
			return false;
		LOG(LogLevel::INFO)<<line;
		ForRequestDivide(line);
		LOG(LogLevel::DEBUG)<<_method<<":"<<_url<<":"<<_version;
		return true;
	}

	std::string& GetUrl()
	{
		return _url;
	}

	std::string IsInput()
	{
		std::string sep="?";
		auto pos=_url.find(sep);
		if(pos==std::string::npos)
			return "";
		std::string ssep="/";
		auto spos=_url.rfind(ssep,pos);
		return _url.substr(spos+ssep.size(),pos-sep.size());
	}

	~HttpRequest()
	{}
private:
	void ForRequestDivide(std::string& str)
	{
		std::stringstream ss(str);
		ss>>_method>>_url>>_version;
	}

	std::string _method;
	std::string _url;
	std::string _version;
	std::unordered_map<std::string,std::string> _kv;
	std::string _blankline;
	std::string _text;
};

class HttpResponse
{
public:
	HttpResponse()
	:_blankline(DAULT_BLANK_LINE)
	,_version("Http/1.1")
	{
		SetTypeMap();
	}

	std::string Serlalize()
	{
		std::string head_str=_version+SPACE_SEP+std::to_string(_code)+SPACE_SEP+_code_des+DAULT_BLANK_LINE;
		std::string kv_str;
		for(auto& x:_kv)
		{
			std::string line=x.first+KV_SEP+x.second+DAULT_BLANK_LINE;
			kv_str+=line;
		}
		return head_str+kv_str+_blankline+_text;
	}

	bool DeSerlalize(std::string& _str)
	{
		return true;
	}

	bool MakeResponse(std::string url)
	{
		_read_file=FILE_PATH+url;
		if(url=="/go.html")
		{
			SetCode(302);
			AddKv("Location","https://www.bilibili.com/");
			return true;
		}
		if(!Tool::ReadFile(FILE_PATH+url,&_text))
		{
			SetCode(404);
			_read_file=FILE_PATH+"/404.html";
			AddKv("Content-Length",std::to_string(Tool::FileSize(_read_file)));
			AddKv("Content-Type",GetType(_read_file));
			LOG(LogLevel::DEBUG)<<GetType(_read_file);
			Tool::ReadFile(_read_file,&_text);
			return false;
		}
		AddKv("Content-Length",std::to_string(Tool::FileSize(_read_file)));
		AddKv("Content-Type",GetType(_read_file));
		LOG(LogLevel::DEBUG)<<GetType(_read_file);
		SetCode(200);
		return true;
	}

	bool MakeResponse(http_func f,std::string url)
	{
		f(url,this);
	}

	void DelToUrl(std::string& url)
	{
		if(url=="/favicon.ico")
		{
			url="/index.html";
		}
	}

	~HttpResponse()
	{}
//private:
	void SetCode(int code)
	{
		_code=code;
		switch(_code)
		{
			case 100:
			{
				_code_des="dealing";
				break;
			}
			case 200:
			{
				_code_des="Ok";
				break;
			}
			case 301:
			{
				_code_des="Moved Permanently";
				break;
			}
			case 302:
			{
				_code_des="See Other";
				break;
			}
			case 404:
			{
				_code_des="Not Found";
				break;
			}
			default:
			{
				_code_des="No Define";
				break;
			}
		}
	}

	void AddKv(std::string key,std::string value)
	{
		_kv[key]=value;
	}

	void SetTypeMap()
	{
		_type_map[".png"]="image/png";
		_type_map[".html"]="text/html";
		_type_map[".jpg"]="image/jpeg";
	}

	std::string GetType(std::string filename)
	{
		auto pos=filename.rfind(".");
		if(pos==std::string::npos)
		{
			return "text/html";
		}
		std::string suffix=filename.substr(pos);
		if(_type_map.find(suffix)==_type_map.end())
		{
			return "text/html";
		}
		return _type_map[suffix];
	}

	std::string _version;
	int _code;
	std::string _code_des;
	std::unordered_map<std::string,std::string> _kv;
	std::string _blankline;
	std::string _text;
	std::string _read_file;
	std::unordered_map<std::string,std::string> _type_map;
	std::string _argv;
};

class Http
{
public:
	Http(uint16_t port)
	:_tcps(std::make_unique<TcpServer>(port))
	{}

	void HttpGetRequest(std::shared_ptr<Socket> sock,InetAddr client)
	{
		std::string package;
		sock->Read(&package);
		HttpRequest hreq;
		hreq.DeSerlalize(package);
		std::string url=hreq.GetUrl();
		HttpResponse hresp;
		hresp.DelToUrl(url);
		std::string func_name=hreq.IsInput();
		if (func_name != "")
		{
			if (_funcs.find(func_name) != _funcs.end())
			{
				hresp.MakeResponse(_funcs[func_name], url);
				sock->Send(hresp.Serlalize());
				return;
			}
		}
		if (!hresp.MakeResponse(url))
		{
			LOG(LogLevel::ERROR) << "制造应答错误";
		}
		sock->Send(hresp.Serlalize());
	}

	void Start()
	{
		_tcps->Start([this](std::shared_ptr<Socket> sock,InetAddr client){
			this->HttpGetRequest(sock,client);
		});
	}

	void Regist(std::string fname,http_func f)
	{
		_funcs[fname]=f;
	}

	~Http()
	{}
private:
	std::unique_ptr<TcpServer> _tcps;
	std::unordered_map<std::string,http_func> _funcs;
};
