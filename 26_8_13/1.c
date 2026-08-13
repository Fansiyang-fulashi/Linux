#include <stdio.h>
#include<unistd.h>

int main()
{
    while(1)
    {
        sleep(1);
        printf("Hello world\n");
    }
    return 0;
}

