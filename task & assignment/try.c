#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>

void sigstop_handler(int signum)
{
	printf("Received SIGSTOP.\n");
}

int main()
{
	pid_t target_pid=fork();
	if(target_pid==0) 
	{
		signal(SIGSTOP,sigstop_handler);
		sleep(5);
	}
	else
	{
		sleep(3);
		int signal_type=SIGSTOP;
		int result=kill(target_pid,signal_type);
		if(result==0)
		{
			printf("signal %d sent to process with pid %d.\n",signal_type,target_pid);
		}
		else
		{
			perror("kill");
		} 
		
		int status;
		pid_t stoppedChild=waitpid(target_pid,&status,WUNTRACED);
		if(WIFSTOPPED(status)) 
		{
			printf("the process %d is stopped.status code %d .\n",target_pid,status);
		}
		else if(WIFCONTINUED(status)) 
		{
			printf("the process %d is cntinued.status code %d .\n",target_pid,status);
		}
		else puts("i don't know");
	}
	
return 0;

}

