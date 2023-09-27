#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"
#define MAX 300

int main(int argc, char *argv[])
{
    //定义一个数组，用于将前面命令的输出存储起来
    char inputBuf[MAX];
    
    // 定义一个数组，用于重构新的参数列表
    char *xargv[MAXARG];

    // 定义一个临时的数组，用于存储分割出来的单词，然后添加到重构新参数的数组的末尾
    //tempNum
    char tempChar[MAX];
    int tempNum = 0;
    char *p = tempChar;

    //先将xargs ...后面参数放入到重构的参数数组中
    int xargvNum = argc - 1;
    for (int i = 0; i < xargvNum; i++)
    {
        xargv[i] = argv[i + 1];
    }

    //定位输入缓冲区中字符位置
    int inputNum = 0;
    while ((inputNum = read(0, inputBuf, sizeof(inputBuf))) > 0)
    {
        //开始对输入缓冲区中的字符串进行分割成单个的单词
        for (int i = 0; i < inputNum; i++){
            //cur跟踪输入缓冲区的每个字符，以便后续判断（判断'\n',' '）
            char cur = inputBuf[i];
            if (cur == '\n'){
                tempChar[tempNum] = 0;
                xargv[xargvNum] = p;
                xargvNum++;
                xargv[xargvNum] = 0;

                int pid = fork();
                if (pid == 0){
                    exec(xargv[0], xargv);
                    exit(0);
                }
                else{
                    wait(0);
                    xargvNum = argc - 1;
                    tempNum = 0;
                    p = tempChar;
                }
            }
            else if(cur == ' '){
                //如果是空格，标志前一个单词结束
                tempChar[tempNum] = 0;
                tempNum++;
                
                //将临时数组的单词添加到重构参数列表的数组xargv末尾
                xargv[xargvNum] = p;
                xargvNum++;
                //更新p指针，使其重新指向临时数组中的新的单词
                p = tempChar + tempNum;
            }
            else{
                //说明cur是单词的一个字母,将其保存到临时数组tempChar中
                tempChar[tempNum] = cur;
                tempNum++;
            }
        }
    }

    exit(0);
}
