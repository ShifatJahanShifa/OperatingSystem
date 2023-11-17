// description: creating 3 processes

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main() 
{
	pid_t pid;
	pid=fork();
	if(pid) 
	{
		fork();
	} 
	printf("hello world\n");
}
