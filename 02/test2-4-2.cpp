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
    int pipe_fd=open("/tmp/test2-4",O_RDONLY);
    char buff[128];
    memset(buff,0,sizeof(buff));
    if(int n = read(pipe_fd,buff,127) > 0)
    {
        cout<<"from 2-4-1:"<<buff<<endl;
    }
    unlink("/tmp/test2-4");
    exit(0);
}
