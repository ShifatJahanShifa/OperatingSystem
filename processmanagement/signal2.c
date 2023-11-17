#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
	pid_t cpid=fork();
	if(cpid>0) 
	{
		sleep(5);
		int result=kill(cpid,SIGINT);
		if(result==0) 
		{
			printf("successfully sent process %d the signal %d\n",cpid,SIGUSR2);     // ask a question
		}
		else perror("kill");
	}
}
