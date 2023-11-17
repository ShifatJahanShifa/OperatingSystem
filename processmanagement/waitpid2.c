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
		printf("child process pid: %d\n",getpid());
		//sleep(3);
		exit(42);
	}
	else if(childPid>0)
	{	
		//sleep(5);
		printf("Parent process pid: %d\n",getpid());
		int status;
		pid_t terminatedChild;
		while((terminatedChild=waitpid(childPid,&status,WNOHANG)))
		{
			if(WIFEXITED(status))
			{
				printf("parent: child process (pid: %d) terminated with status %d\n",terminatedChild,WEXITSTATUS(status));
			}
			else 
			{
				printf("parent: child process (pid: %d) terminated abnormally\n",terminatedChild);
			}
		}
		for(int i=0;i<300;i++) puts("waiting");
		//sleep(5);
	}
	else 
	{
		perror("fork");
		exit(1);
	}
	//sleep(5);
	//for(int i=0;i<10;i++) puts("waiting");
	puts("parent process is done");
	return 0;
}
