#include"mystdio.h"
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

myfile* init(int fd,int flag)
{
    myfile* f=(myfile*)malloc(sizeof(myfile));
    if(f==NULL) return NULL;
    f->fileno=fd;
    f->flag=flag;
    f->buflen=0;
    f->flush_method=LINE_FLASH;
    memset(f->outbuf,0,sizeof(f->outbuf));
    return f;
}

myfile* myopen(const char* filename,const char* mod)
{
    int fd=-1;
    int flag=0;
    if(strcmp(mod,"w")==0)
    {
        flag=O_CREAT|O_WRONLY|O_TRUNC;
        fd=open(filename,flag,0666);
    }
    else if(strcmp(mod,"r")==0)
    {
        flag=O_CREAT|O_WRONLY|O_APPEND;
        fd=open(filename,flag,0666);
    }
    else if(strcmp(mod,"a")==0)
    {
        flag=O_RDWR;
        fd=open(filename,flag);
    }
    else 
    {
        printf("error!!!\n");
        return NULL;
    }
    return init(fd,flag);
}

void myclose(myfile* file)
{
    if(file==NULL) return;
    myflush(file);
    close(file->fileno);
    free(file);
}

int myfwrite(myfile* file,void* str,int len)
{
    memcpy(file->outbuf+file->buflen,str,len);
    file->buflen+=len;
    if((file->flush_method&LINE_FLASH)&&file->outbuf[file->buflen-1]=='\n')
    {
        myflush(file);
    }
    return 0;
}

void myflush(myfile* file)
{
    if(file->buflen<=0) return;
    int n=write(file->fileno,file->outbuf,file->buflen);
    (void)n;
    fsync(file->fileno);
    file->buflen=0;
}
