#include "kernel/types.h"
#include "user/user.h"
#include "stddef.h"

void prime(int fd[2])
{
    int fd_new[2];
    pipe(fd_new);

    close(fd[1]);
    int preNum, nextNum;

    // 在循环中添加条件，检查是否成功读取数据
    while (read(fd[0], &preNum, sizeof(int)) > 0)
    {
        printf("prime %d\n", preNum);

        int pid = fork();
        if (pid > 0)
        {
            // 父进程

            // 新的管道中只往里面写数据给下一个进程，不在该管道读取数据，所以关闭读管道
            close(fd_new[0]);

            // 从旧管道中读取剩余的数据，目的是剔除掉 preNum 的倍数
            while (read(fd[0], &nextNum, sizeof(int)))
            {
                // 当后面的数 nextNum 不是 preNum 的倍数时，写入另一个管道
                if (nextNum % preNum != 0)
                {
                    write(fd_new[1], &nextNum, sizeof(int));
                }
            }

            // 关闭管道
            close(fd[0]);
            close(fd_new[1]);

            // 等待子进程结束
            wait(NULL);

            // 退出循环，因为没有更多的数据需要处理
            break;
        }
        else
        {
            // 子进程
            prime(fd_new);
        }
    }

    // 子进程在这里退出，不再进行递归调用
    exit(0);
}

int main(int argc,char **argv)
{
    if(argc != 1)
    {
        printf("error\n");
        exit(1);
    }

    int fd[2];
    pipe(fd);
    
    int pid = fork();
    if(pid > 0 )
    {
        //父进程
        close(fd[0]);
        for (int i = 2; i < 36; i++)
        {
            write(fd[1], &i, sizeof(int));
        }
        close(fd[1]);
        wait(NULL);
    }
    else{
        //子进程
        prime(fd);
    }
    exit(0);
}