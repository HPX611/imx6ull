#include "stdio.h" 
#include "unistd.h" 
#include "sys/types.h" 
#include "sys/stat.h" 
#include "fcntl.h" 
#include "stdlib.h" 
#include "string.h" 
#include "poll.h" 
#include "sys/select.h" 
#include "sys/time.h" 
#include "linux/ioctl.h" 
#include "signal.h"


#define LEDOFF 0
#define LEDON 1




int main(int argc,char **argv)
{
    char *filename;
    int fd,retvalue;
    unsigned char databuf[2];
    if(argc !=3 )
        return -1;
    
    filename = argv[1];

    fd = open(filename,O_RDWR);

    databuf[0] = atoi(argv[2]);
    retvalue = write(fd,databuf,sizeof(databuf));

    close(fd);
    return 0;
}