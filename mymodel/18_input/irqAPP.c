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
#include <linux/input.h>


static struct input_event inputevent;




int main(int argc,char **argv)
{
    char *filename;
    int fd,retvalue;
    unsigned char databuf[2];
    if(argc !=2 )
        return -1;
    
    filename = argv[1];

    fd = open(filename,O_RDWR);


    while(1)
    {
        read(fd,&inputevent,sizeof(inputevent));
        switch(inputevent.type){
            case EV_KEY:
                printf("EV_KEY: code = %d, value = %d\n",inputevent.code,inputevent.value);
                break;
            case EV_REL:
                break;
            default:
                break;
        }
    }

    close(fd);
    return 0;
}