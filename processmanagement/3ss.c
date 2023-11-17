// use of sigterm, sigfpe, sigusr1

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>

void sig_hand(int signo)
{
	printf("i am here just for testing\n");
}

void sig_hand1(int signo)
{
	//sleep(2);
	printf("i am terminating\n");
}

void sig_hand2(int signo)
{
	printf("can't divide by zero\n");
	exit(0);
}

int main() 
{
	signal(SIGFPE,sig_hand2);
	pid_t pid;
	int status;
	pid=fork();
	
	if(pid==0)
	{
		if(signal(SIGUSR1,SIG_IGN)!=SIG_IGN) 
		{
			signal(SIGUSR1,sig_hand);
		}
		/*if(signal(SIGTERM,SIG_IGN)!=SIG_IGN) 
		{
			signal(SIGTERM,sig_hand1);
		}*/
		//pause();
		sleep(8);
		//sleep(6);
		exit(0);
		
	}
	else 
	{
		sleep(2);
		kill(pid,SIGUSR1);
		sleep(2);
		kill(pid,SIGTERM);
		sleep(3);
		wait(&status);
	}
	
	if(WIFEXITED(status)) printf("normal termination\n");
	else if(WIFSIGNALED(status)) printf("signal\n");
	
	int a=5,b=0;
	int res=a/b;
	return 0;
	
}
