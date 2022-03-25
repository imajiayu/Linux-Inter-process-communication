#include<iostream>
#include<signal.h>
#include<unistd.h>
#include<errno.h>
#include"../../include/my_daemon.h"
#define SIGPRINT 47
#define SIGEXIT 53
using namespace std;

void sig47hander(int)
{
    cout<<"get signal 47"<<endl;
}

void sig53hander(int)
{
    cout<<"get signal 53"<<endl;
    exit(0);
}

int main()
{
    my_daemon();
    signal(SIGPRINT,sig47hander);
    signal(SIGEXIT,sig53hander);
    
    pid_t pid=fork();
    if(pid<0)
    {
        cout<<"fork error,errno:%d"<<errno<<endl;
        exit(0);
    }
    else if(pid>0)
    {
        while (1)
        {
            signal(SIGCHLD,exit);
            kill(pid,SIGPRINT);
            sleep(1);
        }
    }
    else
    {
        while (1)
        {
            sleep(1);
        }
    }
}