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
		pid_t terminatedChild=waitpid(childPid,&status,WNOHANG);
		if(terminatedChild==childPid)
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
		else if(terminatedChild==0) 
		{
			printf("child process %d has not terminated yet\n",childPid); 
		}
		else 
		{
			perror("waitpid");
		}
		sleep(4);
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
