#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define LEDON 1
#define LEDOFF 0

int main(int argc,char **argv){
    int fd = 0;
    
    fd = open(argv[1],O_RDWR);

    write(fd,atoi(argv[2]),1);

    close(fd);
    return 0;
}