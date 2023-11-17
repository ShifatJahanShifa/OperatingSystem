// the problem is: user will input time for executing a child then the child will be stopped, main process will collect the status, thyen again executing the child process. when user will input 0 time for execution, then only thye child process will be be terminated immediately. this problem is actually quite interesting! and i did it! (*-*)


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
	int time,status;
	pid_t pid;
	pid=fork();
	
	if(!pid)
	{
		for(;;)
		{
			puts("I'm child");
			sleep(1);
		}
	}
	else 
	{
		kill(pid,SIGSTOP);
		waitpid(pid,&status,WUNTRACED|WCONTINUED);
				//wait(&status);
		if(WIFSTOPPED(status)) printf("the process %d has been stopped with status %d\n",pid,WSTOPSIG(status));
		else if(WIFCONTINUED(status)) printf("the process %d has been continued.\n",pid);
		
		do
		{
			puts("input time for executing the child process: ");
			scanf("%d",&time);
			if(time>0)
			{
				kill(pid,SIGCONT);
				waitpid(pid,&status,WUNTRACED|WCONTINUED);
				if(WIFSTOPPED(status)) printf("the process %d has been stopped with status %d\n",pid,WSTOPSIG(status));
				else if(WIFCONTINUED(status)) printf("the process %d has been continued.\n",pid);
				
				sleep(time);
				
				kill(pid,SIGSTOP);
				waitpid(pid,&status,WUNTRACED|WCONTINUED);
				if(WIFSTOPPED(status)) printf("the process %d has been stopped with status %d\n",pid,WSTOPSIG(status));
				else if(WIFCONTINUED(status)) printf("the process %d has been continued.\n",pid);
			}
		}while(time>0);
		
		kill(pid,SIGKILL);
		wait(NULL);
	}
}
