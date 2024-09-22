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


int main(int argc,char **argv)
{
    int fd,ret;
    char *filename;
    struct pollfd fds;
    fd_set readfds;
    struct timeval timeout;
    unsigned char data;


    if(argc !=2 )
        return -1;
    
    filename = argv[1];

    fd = open(filename,O_RDWR | O_NONBLOCK);
    if(fd < 0)
    {
        perror("open file error");
        return -1;
    }
#if 0
    //构造结构体
    fds.fd = fd;
    fds.exents = POLLIN;

    while(1){
        ret = poll(&fds,1,500);
        if(ret){
            ret = read(fd,&data,sizeof(data));
            if(data)
                printf("key value: %d\n",data);
        } else if(ret == 0){
            //超时
        } else if(ret < 0){
            //出错
        }
    }
#endif

    while(1){
        FD_ZERO(&readfds);
        FD_SET(fd,&readfds);

        //构造超时时间
        timeout.tv_sec = 0;
        timeout.tv_usec = 500000;
        ret = select(fd+1,&readfds,NULL,NULL,&timeout);
        switch(ret){
            case 0:
                break;
            case -1:
                break;
            default:
                if(FD_ISSET(fd,&readfds)){
                    ret = read(fd,&data,sizeof(data));
                    if(ret < 0){

                    } else {
                        if(data)
                            printf("key value: %s\n",data);
                    }
                }
                break;
        }
    }
    close(fd);
    return 0;
}