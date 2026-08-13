#include"processbar.h"

#define NUM 144
#define ch '#'
#define speed 1

void processbar(int process)
{
    static int cnt=0;
    cnt%=4;
    char buf[NUM+1];
    memset(buf,0,NUM+1);
    char fmt[64];
    char lable[4]={'|','/','-','\\'};
    for(int i=0;i<process;i++)
    {
        buf[i]=ch;
    }
    sprintf(fmt,"[%%-%ds][%%6.2f%%%%][%%c]\r",NUM);
    printf(fmt,buf,process*100/(double)NUM,lable[cnt]);
    cnt++;
    fflush(stdout);
}

void DownLoad()
{
    double cur=0;
    while(cur<=NUM)
    {
        processbar(cur);
        usleep(50000);
        cur+=speed;
    }
    printf("\ndownload done,all %d MB\n",NUM);
}
