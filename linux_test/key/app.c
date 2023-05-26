#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"

int main(int argc, char const *argv[])
{
    int key = open("/dev/key_demo" ,O_RDWR);

    if(key < 0 )
	{
		perror("/dev/demo_dev:");
		return -1;
	}
    char buf[100]= "0";
    printf("in app\n");
    while(1)
    {
        read(key,buf,sizeof(buf));

        if(buf[0]== 0)
        {
            printf("pressed\n");
    
        }
        
    }
    return 0;
}
