#include<iostream>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/socket.h>
#include <sys/un.h>
#include"../../include/my_daemon.h"
using namespace std;

int main()
{
    my_daemon();

    int sockfd=socket(AF_LOCAL,SOCK_STREAM,0);
    if(sockfd<0)
    {
        cout<<"socket error,errno:"<<errno<<endl;
        exit(0);
    }

    struct sockaddr_un servaddr;
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sun_family=AF_LOCAL;
    strcpy(servaddr.sun_path,"/tmp/test6-1");
    
    int ret=connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    if(ret<0)
    {
        cout<<"connect error,errno:"<<errno<<endl;
        exit(0);
    }

    cout<<"client connect successfully"<<endl;
    
    char buf[4096];
    memset(buf,0,sizeof(buf));
    int n=read(sockfd,buf,4096);
    if(n<0)
    {
        cout<<"read error,errno:"<<errno<<endl;
        exit(0);
    }
    buf[n]='\0';
    cout<<"from 6-1-1"<<buf<<endl;

    char sendline[4096];
    memset(sendline,0,sizeof(sendline));
    strcpy(sendline,"here is 6-1-2\n");
    n=write(sockfd,sendline,sizeof(sendline));
    if(n<0)
    {
        cout<<"write error,errno:"<<errno<<endl;
        exit(0);
    }

    close(sockfd);
    exit(0);
}