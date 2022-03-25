#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include"../../include/my_daemon.h"
#include<iostream>
#include<errno.h>
using namespace std;

int main()
{
    my_daemon();
    int ret=mkfifo("/tmp/test2-1",0666);
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
        int pipe_fd=open("/tmp/test2-1",O_RDONLY);
        char buff[128] = {0};
        int n = 0;
        if(n = read(pipe_fd,buff,127) > 0)
        {
            cout<<"from parent:"<<buff<<endl;
        }
    }
    else
    {
        int pipe_fd=open("/tmp/test2-1",O_WRONLY);
        char buff[128] = "here is parent";
        write(pipe_fd,buff,sizeof(buff));
    }
    unlink("/tmp/test2-1");
    exit(0);
}
