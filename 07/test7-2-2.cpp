#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>
using namespace std;

int main(int argc,char** argv)
{
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

    while(true)
    {
        int fd=open(filename,O_RDONLY|O_CREAT|O_NONBLOCK);
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

        if(lock.l_type==F_WRLCK)
            sleep(1);
        else
        {
            lock.l_type=F_RDLCK;
            lock.l_whence=SEEK_SET;
            lock.l_start=0;
            lock.l_len=4096;
            lock.l_pid=getpid();
            
            ret=fcntl(fd,F_SETLK,&lock);
            if(ret<0)
            {
                cout<<"set rdlck error,errno:"<<errno<<endl;
                exit(0);
            }

            char buf[4096];
            memset(buf,0,sizeof(buf));
            int n=read(fd,buf,sizeof(buf));
            if(n<0)
            {
                cout<<"read error,errno:"<<errno<<endl;
                exit(0);
            }
            else if(n==0)
            {
                cout<<"empty"<<endl;
                lock.l_type=F_ULOCK;
                ret=fcntl(fd,F_SETLK,&lock);
                if(ret<0)
                {
                    cout<<"set rdlck error,errno:"<<errno<<endl;
                    exit(0);
                }

                close(fd);
                sleep(1);
                continue;
            }

            cout<<"from 7-1-1:"<<buf<<endl;
            lock.l_type=F_UNLCK;
            ret=fcntl(fd,F_SETLK,&lock);
            if(ret<0)
            {
                cout<<"unlock rdlck error,errno:"<<errno<<endl;
                exit(0);
            }
            break;
        }
    }
    
    exit(0);
}