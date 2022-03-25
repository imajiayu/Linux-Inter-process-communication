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

int is_test3_2_2_running()
{
    FILE * fp;
	char buffer[80];
	fp=popen("ps -ef | grep test3-2-2 | grep -v grep | awk '{print $2}'","r");
	fgets(buffer,sizeof(buffer),fp);
	pclose(fp);

    return atoi(buffer);
}

int main()
{
    my_daemon();
    pid_t pid;
    while(true)
    {
        if(!(pid=is_test3_2_2_running()))
            sleep(1);
        else
            break;
    }
    for(int i=0;i<10;i++)
    {
        if(!(pid=is_test3_2_2_running()))
        {
            sleep(1);
            continue;
        }
        for(int j=0;j<10;j++)
        {  
            if(j==9)
            {
                kill(pid,SIGEXIT);
                exit(0);
            }
            else
            {
                kill(pid,SIGPRINT);
                sleep(1);
            }
            if(!is_test3_2_2_running())
                break;
        }
    }
}