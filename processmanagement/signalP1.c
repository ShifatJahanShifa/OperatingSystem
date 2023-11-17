// send a signal to kill a process

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>


int main()
{
	pid_t pid;
	pid=fork();
	int status;
	
	if(!pid) 
	{
		for(;;) 
		{
			puts("Shifa");
		} 
	} 
	else 
	{
		sleep(1);
		kill(pid,SIGKILL);
		wait(&status); 
	} 
	
	if(WIFEXITED(status)) printf("process %d terminated normally with status %d\n",pid,WEXITSTATUS(status));
	else if(WIFSIGNALED(status)) printf("process %d terminated abnormally with status %d\n",pid,WTERMSIG(status)); 
}
