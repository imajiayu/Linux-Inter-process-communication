#include<iostream>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>
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

    int flags=fcntl(sockfd, F_GETFL, 0);
    fcntl( sockfd, F_SETFL, flags | O_NONBLOCK );

    struct sockaddr_un servaddr;
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sun_family=AF_LOCAL;
    strcpy(servaddr.sun_path,"/tmp/test6-2");

    int connect_flag;
    if( connect_flag=(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) )< 0 && errno!=EINPROGRESS)
    {
        printf("connect error1: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }

    fd_set fs_read;
    FD_ZERO( &fs_read );
    FD_SET( sockfd,&fs_read );

    fd_set fs_write;
    FD_ZERO(&fs_write);
    FD_SET( sockfd,&fs_write );

    fd_set fs_error;
    FD_ZERO(&fs_error);
    FD_SET(sockfd,&fs_error);

    int ret = select( sockfd+1,&fs_read,&fs_write,&fs_error,NULL);
    if( ret <0 )
    {
        printf("select error: %s(errno: %d)",strerror(errno),errno);
        exit(0);
    }

    if( !FD_ISSET( sockfd,&fs_write ) )
    {
        printf("connect2 error: %s(errno: %d)",strerror(errno),errno);
        exit(0);
    }

    int optval = -1;
    socklen_t optlen = sizeof(optval);
    ret = getsockopt( sockfd,SOL_SOCKET,SO_ERROR,&optval,&optlen );
    if( ret != 0 || optval != 0)
    {
        printf("connect error3: %s(errno: %d)",strerror(errno),errno);
        exit(0);
    }
    
    cout<<"client connect successfully"<<endl;

    fd_set readfd;
    fd_set writefd;
    while(true)
    {
        FD_ZERO(&readfd);
        FD_ZERO(&writefd);
        FD_SET(sockfd,&readfd);
        FD_SET(sockfd,&writefd);
        ret=select(sockfd+1,&readfd,NULL,NULL,NULL);

        if(FD_ISSET(sockfd,&readfd))
        {
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
            break;
        }
    }
    
    FD_ZERO(&readfd);
    FD_ZERO(&writefd);
    FD_SET(sockfd,&readfd);
    FD_SET(sockfd,&writefd);
    ret=select(sockfd+1,NULL,&writefd,NULL,NULL);
    if(FD_ISSET(sockfd,&writefd))
    {
        char sendline[4096];
        memset(sendline,0,sizeof(sendline));
        strcpy(sendline,"here is 6-1-2\n");
        int n=write(sockfd,sendline,sizeof(sendline));
        if(n<0)
        {
            cout<<"write error,errno:"<<errno<<endl;
            exit(0);
        }
    }

    close(sockfd);
    exit(0);
}