#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc,char** argv){
    int fd = 0;
    char buf[100];
    int ret = 0;

    if(argc != 3){
        printf("error use!\r\n");
        exit(1);
    }

    fd = open(argv[1],O_RDWR);
    if(fd < 0){
        perror("open():");
        exit(1);
    }


    if(atoi(argv[2]) == 1){
        ret = read(fd,buf,50);
        if(ret < 0){
            perror("read():");
            exit(1);
        }else {
            printf("read data:%s\r\n",buf);
        }
    }

    if(atoi(argv[2]) == 2){
        ret = write(fd,buf,50);
        if(ret < 0){
            perror("write():");
            exit(1);
        }
    }

    if(close(fd) < 0){
        perror("close():");
        exit(1);
    }

    return 0;
}