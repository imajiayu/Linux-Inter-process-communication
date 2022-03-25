#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<fcntl.h>
#include"../../include/my_daemon.h"
#include<iostream>
#include<errno.h>
using namespace std;

int main()
{
    my_daemon();
    int ret=mkfifo("/tmp/test2-3_1",0666);
    if(ret<0)
    {
        cout<<"mkfifo error,errno:"<<errno<<endl;
        exit(0);
    }
    ret=mkfifo("/tmp/test2-3_2",0666);
    if(ret<0)
    {
        cout<<"mkfifo error,errno:"<<errno<<endl;
        exit(0);
    }


    pid_t pid = fork();
    if(pid<0)
    {
        cout<<"fork error,errno:"<<errno<<endl;
        exit(0);
    }
    else if(pid==0)
    {
        int pipe_fd_1=open("/tmp/test2-3_1",O_WRONLY);
        int pipe_fd_2=open("/tmp/test2-3_2",O_RDONLY);
        char buff[128] = "here is child";
        write(pipe_fd_1,buff,sizeof(buff));
        memset(buff,0,sizeof(buff));
        if(int n = read(pipe_fd_2,buff,127) > 0)
        {
            cout<<"from parent:"<<buff<<endl;
        }
    }
    else
    {
        int pipe_fd_1=open("/tmp/test2-3_1",O_RDONLY);
        int pipe_fd_2=open("/tmp/test2-3_2",O_WRONLY);
        char buff[128] = {0};
        int n = 0;
        if(n = read(pipe_fd_1,buff,127) > 0)
        {
            cout<<"from child:"<<buff<<endl;
        }
        memset(buff,0,sizeof(buff));
        strcpy(buff,"here is parent");
        write(pipe_fd_2,buff,sizeof(buff));
    }
    unlink("/tmp/test2-3_1");
    unlink("/tmp/test2-3_2");
    exit(0);
}
