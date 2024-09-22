#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>


#define KEY0VALUE 0XF0
#define INVAKEY 0X00

int main(int argc,char **argv){
    int fd ,ret ;
    char *filename;
    unsigned char  keyvalue;

    if(argc != 2){
        return -1;
    }

    filename = argv[1];

    fd = open(filename,O_RDWR);
    if(fd  < 0){
        return -1;
    }

    while(1){
        read(fd,&keyvalue,sizeof(keyvalue));
        if(keyvalue == KEY0VALUE){
            printf("KEY0 Press , value = %#x\r\n",keyvalue);
        }
    }
    ret = close(fd);
    if(ret<0)   return -1;
    return 0;
}