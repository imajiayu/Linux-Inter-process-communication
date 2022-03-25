#include<unistd.h>
#include"../../include/my_daemon.h"
#include<iostream>
#include<errno.h>
using namespace std;

int main()
{
    my_daemon();
    int fd[2];
    pipe(fd);

    pid_t pid = fork();
    if(pid<0)
    {
        cout<<"fork error,errno:"<<errno<<endl;
        exit(0);
    }
    else if(pid==0)
    {
        close(fd[1]);
        char buff[128] = {0};
        int n = 0;
        if(n = read(fd[0],buff,127) > 0)
        {
            cout<<"from parent:"<<buff<<endl;
        }
        close(fd[0]);
    }
    else
    {
        close(fd[0]);
        char buff[128] = "here is parent";
        write(fd[1],buff,127);
        close(fd[1]);
    }

    exit(0);
}
