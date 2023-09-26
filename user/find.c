#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char *path,char *target)
{
    int fd;
    char *p;
    char buf[512];
    struct dirent de;
    struct stat st;

    // 判断目录是否合法
    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    // 如果不是目录类型
    if (st.type != T_DIR)
    {
        // 报类型不是目录错误
        fprintf(2, "find: %s is not a directory\n", path);
        // 关闭文件描述符 fd
        close(fd);
        return;
    }

    //路径过长，不能存放在缓冲区
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
        printf("find: path too long\n");
        exit(1);
    }

    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';

    while (read(fd, &de, sizeof(de)) == sizeof(de)){
        if (de.inum == 0)
            continue;
        
        //"."  ".." 不要递归
        if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
            continue;

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (stat(buf, &st) < 0){
            printf("find: cannot stat %s\n", buf);
            continue;
        }

        if (st.type == T_DIR){
            find(buf, target);
        }
        else if(st.type==T_FILE && !strcmp(de.name,target)){
            printf("%s\n", buf);
        }
    }
}

int 
main(int argc,char **argv)
{
    if (argc != 3){
        fprintf(2, "error path\n");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}
