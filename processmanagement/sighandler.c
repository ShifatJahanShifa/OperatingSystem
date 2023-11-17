#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>

void sigint(int signum) 
{
	printf("the signal is received %d\n",signum);
	printf("signal received by SIGINT (ctrl+c).\n");
}

int main()
{
	pid_t cpid=fork();
	if(cpid==0) 
	{
		signal(SIGINT,sigint);
		//sleep(5);
	}
	else if(cpid>0) 
	{
		sleep(3);
		int result=kill(cpid,SIGINT);
		if(result==0) 
		{
			printf("signal %d is sent to process %d\n",SIGINT,cpid);
		}
		else perror("kill");
	}
	else perror("fork");

}

