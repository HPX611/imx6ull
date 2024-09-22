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


static int fd = 0;

static void sigio_signal_func(int signum)
{
    int err = 0;
    unsigned int keyvalue = 0;
    err = read(fd,&keyvalue,sizeof(keyvalue));
    if(err < 0){

    } else {
        printf("sigio signal key value = %d\n",keyvalue);
    }
}

int main(int argc,char **argv)
{
    char *filename;
    int flags = 0;

    if(argc !=2 )
        return -1;
    
    filename = argv[1];

    fd = open(filename,O_RDWR);
    
    signal(SIGIO,sigio_signal_func);

    fcntl(fd,F_SETOWN,getpid());
    flags = fcntl(fd,F_GETFD);
    fcntl(fd,F_SETFL,flags|FASYNC);

    while(1){
        sleep(2);
    }

    close(fd);
    return 0;
}