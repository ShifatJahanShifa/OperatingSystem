#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
	pid_t childPid;
	printf("Parent process pid: %d\n",getpid());
	childPid=fork();
	if(childPid==0)
	{
		printf("child process pid: %d\n",getpid());
		exit(42);
	}
	else if(childPid>0)
	{	
		int status;
		pid_t terminatedChild=wait(&status);
		if(WIFEXITED(status))
		{
			printf("parent: child process (pid: %d) terminated with status %d\n",terminatedChild,WEXITSTATUS(status));
		}
		else if(WIFSIGNALED(status))
		{
			printf("parent: child process (pid: %d) terminated due to signal %d\n",terminatedChild,WTERMSIG(status));
		}
		else if(WCOREDUMP(status))
		{
			puts("core dump by child process");
		}
	}
	else 
	{
		perror("fork");
		exit(1);
	}
	puts("parent process is done");
	return 0;
}
