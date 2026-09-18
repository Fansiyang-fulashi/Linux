#include"comm.hpp"

#define gdefaultid -1
#define shmsize 4096

const std::string FATHNAME =".";
const int proid=1;
const int CREATOR=1;
const int USER=2;


#define ERROR_DEAL(x) perror(x); exit(EXIT_FAILURE);

class shm
{
public:
    shm(const int userid)
    :_shmid(gdefaultid)
    ,_size(shmsize)
    ,_userid(userid)
    ,_virual_address(nullptr)
    {
    }

    ~shm()
    {
        shmdt(_virual_address);
        Destory();
    }

    void Create()
    {
        key_t key=ftok(FATHNAME.c_str(),proid);
        if(key<0)
        {
            ERROR_DEAL("ftok");
        }
        if(_userid==CREATOR)
        _shmid=shmget(key,_size,IPC_CREAT|IPC_EXCL|0666);
        else if(_userid==USER)
        _shmid=shmget(key,_size,IPC_CREAT|0666);
        else
        {
            std::cerr<<"userid error"<<std::endl;
            return;
        }
        if(_shmid<0)
        {
            ERROR_DEAL("shmget");
        }
        _virual_address=shmat(_shmid,nullptr,0);
    }

    void Destory()
    {
        if(_userid==CREATOR)
        shmctl(_shmid,IPC_RMID,nullptr);
    }

    void* GetVirualAddress()
    {
        if(_shmid==gdefaultid)
        std::cerr<<"no create"<<std::endl;
        return _virual_address;
    }

private:
    int _shmid;
    size_t _size;
    const int _userid;
    void* _virual_address;
};