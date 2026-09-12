#include <stdio.h>
#include<string.h>

int main(int argc,char* argv[])
{
    if(argc!=2)
    {
        printf("Used Error!\n");
        return 1;
    }
    FILE* pf=fopen(argv[1],"r");
    if(pf==NULL)
    {
        printf("Not Exist File!\n");
        return 1;
    }
    while(1)
    {
        char buf[128];
        memset(buf,0,sizeof(buf));
        int n=fread(buf,1,sizeof(buf)-1,pf);
        if(n>0)
        {
            printf("%s",buf);
        }
        if(feof(pf))
            break;
    }
    fclose(pf);
    return 0;
}















//int main()
//{
//    FILE* pf=fopen("log.txt","w");
//    if(pf==NULL)
//    {
//        perror("fopen");
//        return 1;
//    }
//    for(int i=0;i<10;i++)
//    {
//        char buf[1024];
//        snprintf(buf,sizeof(buf),"msg%d\n",i);
//        fwrite(buf,strlen(buf),1,pf);
//    }
//    fclose(pf);
//    return 0;
//}

