#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>

int main()
{
	pid_t pid;
	int status;
	pid=fork();
	
	if(pid==0)
	{
		//sleep(1);
		  // Child process
		printf("Child process (PID: %d) is running.\n", getpid());
		printf("Child process (PID: %d) will be stopped.\n", getpid());
		sleep(5);
		// Simulate being stopped by the parent
		printf("Child process (PID: %d) has been continued.\n", getpid());
		exit(0);
	}
	else 
	{
		sleep(1);
		kill(pid,SIGSTOP);
		waitpid(pid,&status,WUNTRACED);
		if(WIFSTOPPED(status)) printf("the process id %d is stopped.status %d\n",pid,WSTOPSIG(status));
		sleep(3);
		kill(pid,SIGCONT);
		printf("continued the child %d id\n",pid);
		waitpid(pid,&status,WCONTINUED);
		if(WIFCONTINUED(status)) printf("the process id %d is continued.\n",pid);
		wait(NULL);
	}
}
