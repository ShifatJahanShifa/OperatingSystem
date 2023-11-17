#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
	pid_t pid;
	pid=fork();
	
	if(!pid) 
	{
		printf("my process id: %d\n",getpid());
		printf("my parent process id: %d\n",getppid());
	}
	else if(pid>0) 
	{
		sleep(2);
		printf("my process id: %d\n",getpid());
		printf("my child process id: %d\n",pid);
		printf("my parent process id: %d\n",getppid());
	}	
	else if(pid==-1) 
	{
		perror("fork");
	}
}


