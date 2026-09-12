#pragma once
#include<stdio.h>
#include<string.h>
#define BUF_MAX_LEN 256

#define NONE_FLASH (1<<0)
#define LINE_FLASH (2<<0)
#define FULL_FLASH (3<<0)

typedef struct myfile
{
    int fileno;
    int flag;
    char outbuf[BUF_MAX_LEN];
    int buflen;
    int flush_method;
}myfile;

myfile* myopen(const char* filename,const char* mod);
void myclose(myfile* file);
int myfwrite(myfile* file,void* str,int len);
void myflush(myfile* file);


