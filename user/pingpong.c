#include "kernel/types.h"
#include "user/user.h"
#include "stddef.h"

int main(int argc, char **argv)
{
    // 创建一对管道，一根用于父进程向子进程发送信息，一根用于子进程向父进程发送信息
    int p_c[2], c_p[2];
    pipe(p_c);
    pipe(c_p);

    char buf[30];
    // 创建子进程
    int pid = fork();
    if (pid == 0)
    { // 子进程

        // 关闭不需要的管道
        close(p_c[1]);
        close(c_p[0]);

        // 子进程从p_c管道读取信息
        if (read(p_c[0], buf, 4) == -1)
        {
            printf("Error: read\n");
            exit(1);
        }
        printf("%d: received %s\n", getpid(), buf);

        // 子进程向c_p管道写入信息
        if (write(c_p[1], "pong", strlen("pong")) == -1)
        {
            printf("Error: wirte\n");
            exit(1);
        }

        // 关闭管道
        close(p_c[0]);
        close(c_p[1]);

        exit(0);
    }
    else
    { // 父进程

        // 关闭不需要的管道
        close(c_p[1]);
        close(p_c[0]);

        // 父进程向p_c管道写入信息
        if (write(p_c[1], "ping", strlen("ping")) == -1)
        {
            printf("Error: wirte\n");
            exit(1); // 异常退出
        }

        wait(NULL);
        // 父进程从c_p管道读取信息
        if (read(c_p[0], buf, 5) == -1)
        {
            printf("Error: read\n");
        }
        printf("%d: received %s\n", getpid(), buf);
        // 关闭管道
        close(c_p[0]);
        close(p_c[1]);
    }

    exit(0);
}