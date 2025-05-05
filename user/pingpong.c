//user/pingpong.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main (int argc, char **argv){
    //创建两个管道：p1 用于父进程向子进程的通信，p2 用于子进程向父进程的通信
    int p1[2], p2[2];
    pipe(p1);
    pipe(p2);

    int pid = fork();
    if (pid < 0){
        //fork失败
        fprintf(2, "fork failed\n");
        
        exit(1);
    }
    else if(pid == 0){
        //子进程
        char buf[1];
        //从父到子的管道中读取
        read(p1[0], buf, 1);
        printf("%d: received ping\n", getpid());

        // 向父发送pong
        write(p2[1], buf, 1);

        exit(0);
    }
    else{
        char buf[1] = {'A'};//任意一个字节

        //向子进程发送ping
        write(p1[1], buf, 1);

        //等子进程回复 pong
        read(p2[0], buf, 1);
        printf("%d: received pong\n", getpid());

        //等待子进程退出
        wait(0);

        exit(0);
    }


}
