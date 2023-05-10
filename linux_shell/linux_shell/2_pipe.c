#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char const *argv[])
{
    
    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    if(pid > 0) //父进程
    {
        close(fd[0]);
        char buf[128];
        while(1)
        {
        printf("父进程写数据\n");
        
        read(0,buf,sizeof(buf));
        write(fd[1],buf,strlen(buf));
        printf("写入完成\n");
        if(wait(NULL)) 
        {
            close(fd[1]);
            printf("父进程进 while \n");
            break;
        }
        }
        return 0;

    }
    else if(pid == 0) //子进程
    {
        close(fd[1]);
        char buf[128];
        read(fd[0],buf,sizeof(buf));
        while(strcmp(buf,"goodbye"))
        {
            sleep(3);
             printf("received the message:%s\n",buf); 
             read(fd[0],buf,sizeof(buf));        
        }
        printf("子进程出 while \n");
        close(fd[0]);
        _exit(-1);
    }

}
