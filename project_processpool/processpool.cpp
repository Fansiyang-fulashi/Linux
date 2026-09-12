#include"processpool.hpp"

int main()
{
    processpool pp(10);
    pp.Init();
    for(int i=0;i<5;i++)
    {
        pp.Run();
        sleep(1);
    }
    pp.CloseAllFile();
    pp.WaitAllProcess();
    return 0;
}