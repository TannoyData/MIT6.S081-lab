#include "kernel/types.h"
#include "user/user.h"
#include <unistd.h>

int
main(int argc,char **argv)
{
    if(argc != 1)
    {
        printf("Error\n");
        exit(1);
    }
    //定义文件描述符，创建管道
    int fd[2];
    pipe(fd);


}