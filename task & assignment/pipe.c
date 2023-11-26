#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>

#define bufferSize 25

int main()
{
	char writeMsg[bufferSize]="greetings from shifa";
	char readMsg[bufferSize];
	int fd[2];
	pid_t pid;
	
	// create pipe
	if(pipe(fd)==-1) 
	{
		fprintf(stderr,"pipe failed");
		return 1;
	} 
	
	pid=fork();
	
	if(pid<0) 
	{
		fprintf(stderr,"fork failed");
		return 1; 
	} 
	else if(pid>0) 
	{
		close(fd[0]);
		
		write(fd[1],writeMsg,strlen(writeMsg)+1);
		close(fd[1]);
	}
	else 
	{
		close(fd[1]);
		
		read(fd[0],readMsg,bufferSize);
		printf("msg = %s \n",readMsg);
		close(fd[0]);
	}
	
	return 0;
}
