// forking multiple times, thereby creating 2^(number of fork) processes
// i love that problem personally cause it reveals a lot of insights about fork and process


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>


int main()
{
	pid_t pid1,pid2;
	pid1=fork();
	pid2=fork();
	
	// four processes here are actually
	if(pid1==0 && pid2==0) 
	{
		printf("i'm the process y\n");
	}
	else if(pid1==0 && pid2)
	{
		printf("i'm the process x\n");
	}
	else if(pid1 && pid2==0)
	{
		printf("i'm the process z\n");
	}
	else 
	{
		printf("i'm the parent process!!\n");
	}
	
	// waiting
	while(wait(NULL)!=-1 || errno!=ECHILD) printf("i'm waiting for my child\n");
}


