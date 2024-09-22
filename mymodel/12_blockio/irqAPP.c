#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define CLOSE_CMD (_IO(0XEF, 0x1)) /* 关闭定时器 */ 
#define OPEN_CMD (_IO(0XEF, 0x2)) /* 打开定时器 */ 
#define SETPERIOD_CMD (_IO(0XEF, 0x3)) /* 设置定时器周期命令 */ 

int main(int argc,char **argv)
{
    int fd,ret;
    char *filename;
    unsigned int data;

    if(argc !=2 )
        return -1;
    
    filename = argv[1];

    fd = open(filename,O_RDWR);
    if(fd < 0)
    {
        perror("open file error");
        return -1;
    }

    while(1){
        ret = read(fd,&data,sizeof(data));
        if(ret < 0){

        }
        else {
            if(data)
                printf("key value: %d\n",data);
        }
    }
    close(fd);
    return 0;
}