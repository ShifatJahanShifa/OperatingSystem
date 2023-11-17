#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>

int main()
{
	pid_t pid;
	pid=fork();
	int status;
	
	if(pid==0) 
	{
		sleep(1);
		abort();
	}
	else 
	{
		wait(&status);
		if(WIFEXITED(status)) printf("id %d, terminated normally\n",pid);
		else if(WIFSIGNALED(status)) printf("id %d, terminated abnormally %d.\n",pid,status);
	}
	
}
