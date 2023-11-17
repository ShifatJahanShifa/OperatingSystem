#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h> 

void hand(int signo)
{
	printf("i AM terminating\n");
}
int main()
{
	puts("before");
	//raise(SIGTERM);
	pid_t pid=fork();
	if(!pid) 
	{
		signal(SIGTERM,hand);
		sleep(2);
		exit(0);
	}
	sleep(2);
	kill(pid,SIGTERM);
	wait(NULL);
	
}
