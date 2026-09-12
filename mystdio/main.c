#include"mystdio.h"

int main()
{
    myfile* f=myopen("log.txt","w");
    char* ch="hello file!\n";
    myfwrite(f,ch,strlen(ch));
    printf("%d",f->fileno);
    printf("%s",f->outbuf);
    myclose(f);
}
