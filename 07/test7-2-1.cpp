#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include "../../include/my_daemon.h"
using namespace std;

int main(int argc,char** argv)
{
    my_daemon();

    char* filename;
    if(argc<=2)
    {
        cout<<"missing parameter!"<<endl;
        exit(0);
    }

    for(int i=1;i<argc;i+=2)
    {
        if(strcmp(argv[i],"--file")==0)
        {
            filename=new char[sizeof(argv[i+1])];
            strcpy(filename,argv[i+1]);
            break;
        }
    }

    int fd=open(filename,O_WRONLY|O_CREAT|O_NONBLOCK);
    if(fd<0)
    {
        cout<<"open error,errno:"<<errno<<endl;
        exit(0);
    }

    struct flock lock;
    bzero(&lock,sizeof(lock));
    int ret=fcntl(fd,F_GETLK,&lock);
    if(ret<0)
    {
        cout<<"fcntl error,errno:"<<errno<<endl;
        exit(0);
    }

    while(lock.l_type!=F_UNLCK)
        sleep(1);

    lock.l_type=F_WRLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=0;
    lock.l_len=4096;
    lock.l_pid=getpid();

    ret=fcntl(fd,F_SETLK,&lock);
    if(ret<0)
    {
        cout<<"set wrlck error,errno:"<<errno<<endl;
        exit(0);
    }
    
    char sendline[4096];
    memset(sendline,0,sizeof(sendline));
    strcpy(sendline,"here is 7-1-1");
    ret=write(fd,sendline,sizeof(sendline));
    if(ret<0)
    {
        cout<<"write error,errno:"<<errno<<endl;
        exit(0);
    }

    lock.l_type=F_UNLCK;
    ret=fcntl(fd,F_SETLK,&lock);
    if(ret<0)
    {
        cout<<"unlock wrlck error,errno:"<<errno<<endl;
        exit(0);
    }

    exit(0);
}