#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<iostream>
#include<string.h>
#include<errno.h>
#include"../../include/my_daemon.h"
using namespace std;

struct shared_use_st
{  
    int written;
    char text[4096];
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

    int id=shmget(key,sizeof(struct shared_use_st),IPC_CREAT|0666);
    if(id<0)
    {
        cout<<"shmget error,errno:"<<errno<<endl;
        exit(0);
    }

    void* shm=NULL;
    shm=shmat(id,NULL,0);
    if(shm == (void*)-1)   
    {  
        cout<<"shmat error,errno:"<<errno<<endl;
        exit(0);
    }

    sleep(1);
    struct shared_use_st *shared=(struct shared_use_st*)shm;
    while(shared->written==1)
    {
        sleep(1);
    }
    strcpy(shared->text,"here is 5-2-2");
    shared->written = 1;

    if(shmdt(shm) == -1)   
    {      
        cout<<"shmdt error,errno:"<<errno<<endl;
        exit(0);
    }  

    exit(0);
}