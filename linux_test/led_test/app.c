#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    int fd = open("/dev/demo_dev.txt" ,O_RDWR);
    if(fd < 0)
	{
		perror("/dev/demo_dev:");
		return -1;
	}
    char buf[100]= "0";
    printf("in app\n");
    while(1)
    {
        printf("in while");
        buf[0] = '0';
        write(fd, buf, strlen(buf)+1);
        sleep(1);
        buf[0] = '1';
        write(fd, buf, strlen(buf)+1);
        sleep(1);
    }
    return 0;
}
