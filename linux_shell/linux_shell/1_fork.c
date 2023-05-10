#include <stdio.h>
#include <unistd.h>
int i=0;
int main(int argc, char const *argv[])
{
    pid_t pid;
    
    pid = fork();
    if(pid < 0)
    {
        perror("fail to fork");
        return -1;
    }
    else if(pid == 0)
    { printf("this is son process\n");
        printf("11111111bye\n");
        printf("*******************\n");
        
    }
        pid = fork();
    if(pid < 0)
    {
        perror("fail to fork");
        return -1;
    }
    else if(pid == 0)
    { printf("this is son process\n");
        printf("2222222bye\n");
        printf("-----------------------\n");
        
    }
    printf("this is parent process\n");
    printf("hello %d\n",i++);
    while(1);
    return 0;
}
