// description: creating a child process; the child process should print 1 through 5 and the parent process should print from 6 through 10.
// i have used here a custom status just for fun <_>
// includes smart coding style, i guess (*_*)

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
	int n;
	if(!pid) 
	{
		n=1;
		printf("my id is: %d\n",getpid());
	} 
	else
	{
		n=6;
		int p=wait(&status);
		printf("\n%d terminated now\n",p);
	} 
	
	for(int i=n;i<n+5;i++) 
	{
		printf("%d ",i);
	}
	if(pid) puts("");
}
