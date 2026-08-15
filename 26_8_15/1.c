#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(int argc,char* args[],char* env[])
{
    (void)argc;
    (void)args;
    (void)env;
    char* value=getenv("USER");
    if(value==NULL)
    {
        printf("read error!\n");
        return 1;
    }
    else if(strcmp(value,"ubuntu")==0)
    {
        printf("welcome!\n");
        //
    }
    else
    {
        printf("you are not real!\n");
        return 1;
    }

    //if(argc!=2)
    //{
    //    printf("error:must have -a/-b/-c\n");
    //    return 0;
    //}
    //if(strcmp(args[1],"-a")==0)
    //{
    //    printf("this is function one\n");
    //}
    //else if(strcmp(args[1],"-b")==0)
    //{
    //    printf("this is function two\n");
    //}
    //else if(strcmp(args[1],"-c")==0)
    //{
    //    printf("this is function three\n");
    //}
    //else 
    //{
    //    printf("Error:must bewteen -a/-b/-c\n");
    //}
    return 0;
}
