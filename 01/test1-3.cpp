#include<unistd.h>
#include"../../include/my_daemon.h"
#include<iostream>
#include<errno.h>
using namespace std;

int main()
{
    my_daemon();
    int fd[2],fd2[2];
    pipe(fd);
    pipe(fd2);

    pid_t pid = fork();
    if(pid<0)
    {
        cout<<"fork error,errno:"<<errno<<endl;
        exit(0);
    }
    else if(pid==0)
    {
        close(fd[0]);
        close(fd2[1]);
        char buff[128] = "here is child";
        write(fd[1],buff,127);
        char readline[128] = {0};
        int n = 0;
        if(n = read(fd2[0],readline,127) > 0)
        {
            cout<<"from parent:"<<readline<<endl;
        }
        close(fd[1]);
        close(fd2[0]);
    }
    else
    {
        close(fd[1]);
        close(fd2[0]);
        char buff[128] = "here is parent";
        write(fd2[1],buff,127);
        char readline[128] = {0};
        int n = 0;
        if(n = read(fd[0],readline,127) > 0)
        {
            cout<<"from child:"<<readline<<endl;
        }
        close(fd[0]);
        close(fd2[1]);
    }
    exit(0);
}
