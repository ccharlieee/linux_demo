#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char const *argv[])
{
    mkfifo("myfifo", 0666);
    int fd = open("myfifo",O_WRONLY);
    if(fd < 0)
    {
        perror("open");
    }
    printf("写端打开成功");

    while(1)
    {
        char buf[128]="";
        printf("Bob:");
        fgets(buf, sizeof(buf), stdin);
        buf[strlen(buf)-1] = '\0';
        write(fd, buf, strlen(buf));
        
        if(strcmp(buf,"bye") == 0)
            break;
    }
    close(fd);
    return 0;
}
