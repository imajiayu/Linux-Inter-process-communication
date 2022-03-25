#include<iostream>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<sys/socket.h>
#include <sys/un.h>
#include"../../include/my_daemon.h"
using namespace std;

int main()
{
    my_daemon();

    int listenfd=socket(AF_LOCAL,SOCK_STREAM,0);
    if(listenfd<0)
    {
        cout<<"socket error,errno:"<<errno<<endl;
        exit(0);
    }

    struct sockaddr_un servaddr,cliaddr;
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sun_family=AF_LOCAL;
    strcpy(servaddr.sun_path,"/tmp/test6-1");
    
    int ret=bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    if(ret<0)
    {
        cout<<"bind error,errno:"<<errno<<endl;
        exit(0);
    }

    listen(listenfd,10);

    cout<<"waiting for connect"<<endl;


    socklen_t len = sizeof(cliaddr);
    int connfd=accept(listenfd,(struct sockaddr *)&cliaddr,&len);
    if(connfd<0)
    {
        cout<<"accept error,errno:"<<errno<<endl;
        exit(0);
    }

    cout<<"server connect successfully"<<endl;

    char sendline[4096];
    memset(sendline,0,sizeof(sendline));
    strcpy(sendline,"here is 6-1-1\n");
    int n=write(connfd,sendline,sizeof(sendline));
    if(n<0)
    {
        cout<<"write error,errno:"<<errno<<endl;
        exit(0);
    }

    char buf[4096];
    memset(buf,0,sizeof(buf));
    n=read(connfd,buf,4096);
    if(n<0)
    {
        cout<<"read error,errno:"<<errno<<endl;
        exit(0);
    }
    buf[n]='\0';
    cout<<"from 6-1-2"<<buf<<endl;

    close(listenfd);
    close(connfd);
    exit(0);
}