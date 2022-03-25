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
    int ret=mkfifo("/tmp/test2-4",0666);
    if(ret<0)
    {
        cout<<"mkfifo error,errno:"<<errno<<endl;
        exit(0);
    }


    int pipe_fd=open("/tmp/test2-4",O_WRONLY);
    char buff[128] = "here is 2-4-1";
    ret=write(pipe_fd,buff,sizeof(buff));
    unlink("/tmp/test2-4");
    exit(0);
}
