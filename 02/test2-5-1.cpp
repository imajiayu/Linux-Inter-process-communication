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
    int ret=mkfifo("/tmp/test2-5_1",0666);
    if(ret<0)
    {
        cout<<"mkfifo error,errno:"<<errno<<endl;
        exit(0);
    }
    ret=mkfifo("/tmp/test2-5_2",0666);
    if(ret<0)
    {
        cout<<"mkfifo error,errno:"<<errno<<endl;
        exit(0);
    }

    int pipe_fd_1=open("/tmp/test2-5_1",O_WRONLY);
    int pipe_fd_2=open("/tmp/test2-5_2",O_RDONLY);
    char buff[128] = "here is 2-5-1";
    ret=write(pipe_fd_1,buff,sizeof(buff));
    memset(buff,0,sizeof(buff));
    if(int n = read(pipe_fd_2,buff,127) > 0)
    {
        cout<<"from 2-5-2:"<<buff<<endl;
    }
    unlink("/tmp/test2-5_1");
    unlink("/tmp/test2-5_2");
    exit(0);
}
