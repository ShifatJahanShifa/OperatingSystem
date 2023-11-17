// description: creating a child process; the child process should print 1 through 5 and the parent process should print from 6 through 10.
// i have used here a custom status just for fun <_>

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

#define WHY_ME 1301

int main() 
{
	int status;
	pid_t pid;
	pid=fork();
	if(!pid) 
	{
		for(int i=1;i<6;i++) 
		{
			printf("child = %d\n",i);
		}
		exit(WHY_ME);
	} 
	wait(&status); 
	
	if(WIFEXITED(status)) printf("process %d terminated normally with status %d\n",pid,WEXITSTATUS(status));
	else if(WIFSIGNALED(status)) printf("process %d terminated abnormally with status %d\n",pid,WTERMSIG(status));
	
	for(int i=6;i<11;i++) 
	{
		printf("parent = %d\n",i);
	}
}
