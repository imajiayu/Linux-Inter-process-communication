#include<iostream>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
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

    int flags=fcntl(listenfd, F_GETFL, 0);
    fcntl( listenfd, F_SETFL, flags | O_NONBLOCK );

    struct sockaddr_un servaddr,cliaddr;
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sun_family=AF_LOCAL;
    strcpy(servaddr.sun_path,"/tmp/test6-2");
    
    int ret=bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    if(ret<0)
    {
        cout<<"bind error,errno:"<<errno<<endl;
        exit(0);
    }

    listen(listenfd,10);

    cout<<"waiting for connect"<<endl;

    fd_set readfd;

    FD_ZERO(&readfd);
    FD_SET(listenfd,&readfd);
    int maxfd=listenfd;
    ret=select(maxfd+1,&readfd,NULL,NULL,NULL);
    if(ret<0)
    {
        printf("select error: %s(errno: %d)",strerror(errno),errno);
        exit(0);
    }
    else if(ret==0)
        ;

    int connfd;
    if(FD_ISSET(listenfd,&readfd))
    {
        socklen_t len = sizeof(cliaddr);
        connfd=accept(listenfd,(struct sockaddr *)&cliaddr,&len);
        if(connfd<0)
        {
            cout<<"accept error,errno:"<<errno<<endl;
            exit(0);
        }

        maxfd=max(maxfd,connfd);

        flags=fcntl(connfd, F_GETFL, 0);
        fcntl( connfd, F_SETFL, flags | O_NONBLOCK );
    }

    cout<<"server connect successfully"<<endl;


    fd_set writefd;
    FD_ZERO(&readfd);
    FD_ZERO(&writefd);
    FD_SET(connfd,&readfd);
    FD_SET(connfd,&writefd);
    ret=select(connfd+1,NULL,&writefd,NULL,NULL);
    if(FD_ISSET(connfd,&writefd))
    {
        char sendline[4096];
        memset(sendline,0,sizeof(sendline));
        strcpy(sendline,"here is 6-2-1");
        int n=write(connfd,sendline,sizeof(sendline));
        if(n<0)
        {
            cout<<"write error,errno:"<<errno<<endl;
            exit(0);
        }
    }
    
    while(true)
    {
        FD_ZERO(&readfd);
        FD_ZERO(&writefd);
        FD_SET(connfd,&readfd);
        FD_SET(connfd,&writefd);
        ret=select(maxfd+1,&readfd,NULL,NULL,NULL);
        if(FD_ISSET(connfd,&readfd))
        {
            char buf[4096];
            memset(buf,0,sizeof(buf));
            int n=read(connfd,buf,4096);
            if(n<0)
            {
                cout<<"read error,errno:"<<errno<<endl;
                exit(0);
            }
            buf[n]='\0';
            cout<<"from 6-1-2"<<buf<<endl;
            break;
        }
    }

    close(listenfd);
    close(connfd);
    exit(0);
}