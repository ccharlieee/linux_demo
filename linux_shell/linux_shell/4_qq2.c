#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>



int main(int argc, char const *argv[])
{
    mkfifo("1to2", 0666);
    mkfifo("2to1", 0666);

    int i = 0;
    for( ;i <2; i++)
    {
        pid_t pid = fork();
        if(pid == 0)
            break;
    }

    if(i == 0) //子进程1 
    {
        int fd = open("2to1",O_WRONLY);
        while(1)
    {
        char buf[128]="";
        char message[128]="Lucy:";
        fgets(buf, sizeof(buf), stdin);
        strcat(message,buf);
        message[strlen(message)-1] = '\0';
        write(fd, message, strlen(message));       
        if(strstr(message,"bye"))
            break;
    }
    close(fd);
    _exit(-1);
    }
    else if(i == 1)//子进程2
    {
        
        int fd = open("1to2",O_RDONLY);
        while(1)
        {
            char buf[128]="";
            read(fd, buf, sizeof(buf));
            printf("%s\n", buf);
            if(strstr(buf,"bye"))
                break;
        }
        close(fd);
        _exit(-1);

    }
    else if(i == 2)//父进程
    {
        while(1)
        {
            pid_t pid = waitpid(-1, NULL, WNOHANG);
            if(pid < 0)
            break;
        }
        
    }
    return 0;
}
