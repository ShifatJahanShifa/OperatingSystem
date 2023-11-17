#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
	pid_t childPid;
	childPid=fork();
	if(childPid==0)
	{
		puts("child process is running");
		sleep(2);
		puts("child process is done");
		exit(0);
	}
	else if(childPid>0)
	{
		printf("parent process created child process with pid: %d\n",childPid);
		sleep(4);
		puts("parent process is woke up");
		
	}
	else 
	{
		perror("fork");
		exit(1);
	}
	puts("parent process is done");
	return 0;
}
