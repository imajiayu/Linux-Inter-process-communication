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
    int pipe_fd_1=open("/tmp/test2-5_1",O_RDONLY);
    int pipe_fd_2=open("/tmp/test2-5_2",O_WRONLY);
    char buff[128];
    if(int n = read(pipe_fd_1,buff,127) > 0)
    {
        cout<<"from 2-5-1:"<<buff<<endl;
    }
    memset(buff,0,sizeof(buff));
    strcpy(buff,"here is 2-5-2");
    write(pipe_fd_2,buff,sizeof(buff));
    unlink("/tmp/test2-5_1");
    unlink("/tmp/test2-5_2");
    exit(0);
}
