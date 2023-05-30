#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "sys/ioctl.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"
#include <poll.h>
#include <sys/select.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
    int i2c = open("/dev/ap3216_demo" ,O_RDWR);
    int ret;
    unsigned short ir,als,ps;

    if(i2c < 0 )
	{
		perror("open failed");
		return -1;
	}
    unsigned short buf[3];
    printf("in app\n");
    
       ret = read(i2c,buf,sizeof(buf));
       if(ret == 0)
       {
        ir = buf[0];
        als = buf[1];
        ps = buf[2]; 
        printf("ir = %d , als = %d , ps = %d \n",ir,als,ps);
       }
       else {
        printf("error read");
       }
       close(i2c);

    return 0;
}
