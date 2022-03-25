#include<unistd.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../../include/my_daemon.h"
#include<iostream>
#include<errno.h>
using namespace std;

struct mymesg
{
    long int mtype;
    char mtext[4096];
};

int main()
{
    my_daemon();

    key_t key=ftok("/tmp",66);
    if(key<0)
    {
        cout<<"ftok error,errno:"<<errno<<endl;
        exit(0);
    }

    int id=msgget(key,IPC_CREAT|0666);
    if(id<0)
    {
        cout<<"msgget error,errno:"<<errno<<endl;
        exit(0);
    }

    pid_t pid=fork();
    if(pid<0)
    {
        cout<<"fork error,errno:"<<errno<<endl;
        exit(0);
    }
    else if(pid>0)
    {
        struct mymesg sendline;
        char msg[4096];
        memset(msg,0,sizeof(msg));
        strcpy(msg,"here is parent");

        sendline.mtype=1;
        strcpy(sendline.mtext,msg);

        if(msgsnd(id,(void *)&sendline,4096,0) < 0)
		{
			cout<<"msgsnd error,errno:"<<errno<<endl;
            exit(0);
		}
    }
    else
    {
        struct mymesg readline;
        char msg[4096];
        memset(msg,0,sizeof(msg));
        if(msgrcv(id,(void*)&readline,4096,1,0) < 0)
		{
			cout<<"read error,errno:"<<errno<<endl;
            exit(0);
		}
        cout<<"from parent:"<<readline.mtext<<endl;

        if(msgctl(id,IPC_RMID,NULL) < 0)
	    {
            cout<<"del error,errno:"<<errno<<endl;
            exit(0);
	    }
    }
    exit(0);
}


