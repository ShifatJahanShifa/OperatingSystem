// use of sigint, sigtstp, sigquit

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>

void sig_hand(int signo)
{
	switch(signo)
	{
		case SIGINT: printf("interrupted by signal\n");
		break;
		
		case SIGTSTP: printf("gave me suspension\n");
		break;
		
		case SIGQUIT: printf("told me to quit\n");
		break;
			
	}
}

int main() 
{
	pid_t pid;
	int status;
	pid=fork();
	
	if(pid==0)
	{
		if(signal(SIGINT,SIG_IGN)!=SIG_IGN) 
		{
			signal(SIGINT,sig_hand);
		}
		if(signal(SIGQUIT,SIG_IGN)!=SIG_IGN) 
		{
			signal(SIGQUIT,sig_hand);
		}
		if(signal(SIGTSTP,SIG_IGN)!=SIG_IGN) 
		{
			signal(SIGTSTP,sig_hand);
		}
		sleep(3);
		exit(0);
		
	}
	else 
	{
		sleep(3);
		//kill(pid,SIGINT);
		//kill(pid,SIGQUIT);
		//kill(pid,SIGTSTP);
	}
	wait(NULL);
	//for(;;) pause();
}
