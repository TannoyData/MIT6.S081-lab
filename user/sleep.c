//sleep.c 2023-9-25

#include "kernel/types.h"
#include "user/user.h"

int 
main(int argc,char **argv)
{
    //如果命令参数个数不等于2，则错误退出
    if(argc != 2)
    {
        //将错误信息输出到标准错误输出
        write(2, "Usage:sleep time\n", strlen("Usage:sleep time\n"));
    }

    //将字符串类型转换成整数类型
    int time = atoi(argv[1]);

    //// 调用系统调用 sleep 函数，传入整型参数
    sleep(time);

    //正常退出
    exit(0);
}