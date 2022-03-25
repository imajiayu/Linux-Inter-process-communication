#include<iostream>
#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<string.h>
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
    while (true)
    {
        sleep(1);
    }
    exit(0);
}