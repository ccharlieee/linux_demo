#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    int fd = open("dev/led_test" ,O_RDWR);
    char buf[100]= "0";
    while(1)
    {
        buf[0] = '0';
        write(fd, buf, strlen(buf)+1);
        sleep(1);
        buf[0] = '1';
        write(fd, buf, strlen(buf)+1);
        sleep(1);
    }
    return 0;
}
