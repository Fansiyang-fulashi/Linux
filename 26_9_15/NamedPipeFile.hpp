#include"comm.hpp"

#define INITFD -1
const int READER=1;
const int WRITER=2;

class npf
{
public:
    npf(const std::string& name,const int id)
    :_name(name)
    ,_fd(INITFD)
    ,_id(id)
    {
        _ref_count[_name]++;
        if(_id==WRITER)
        {
            int mkfifor=mkfifo(_name.c_str(),0666);
            if(mkfifor<0) 
            {
            std::cerr<<"mkfifo wrong"<<std::endl;
            return; 
            }
        }
    }

    ~npf()
    {
        if(_fd>0)
        close(_fd);
        _ref_count[_name]--;
        if(_ref_count[_name]==0)
        unlink(_name.c_str());
    }

    int GetWriteFd()
    {
        if(_id!=WRITER)
        {
            std::cerr<<"id error"<<std::endl;
            return -1;
        }
        _fd=open(_name.c_str(),O_CREAT|O_WRONLY|O_TRUNC);
        if(_fd<0)
        {
            std::cerr<<"open fifofile wrong"<<std::endl;
            return -1;
        }
        return _fd;
    }

    int GetReadFd()
    {
        if(_id!=READER)
        {
            std::cerr<<"id error"<<std::endl;
            return -1;
        }
        _fd=open(_name.c_str(),O_RDONLY);
        if(_fd<0)
        {
            std::cerr<<"open fifofile wrong"<<std::endl;
            return -1;
        }
        return _fd;
    }

    void GetFileInf()
    {
        std::cout<<_fd<<std::endl;
        std::cout<<_name<<std::endl;
    }
private:
    int _fd;
    std::string _name;
    const int _id;
    std::unordered_map<std::string,int> _ref_count;
};