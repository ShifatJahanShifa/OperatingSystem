// the problem is: user will be asked a simple question. if the answer is correct, then the  game  is closed. otherwise the user will be provided with a hint after 5 seconds. and it is assumed that after having the hint, the user will be successful in providing correct answer. well this program can be written more optimizely.....I just wanted to learn the coding and playing with signal. that's it.


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>

int ans=0;

void null_handler(int signo)
{
	sleep(5);
}

void sigusr1_handler(int signo)
{
	printf("the answer is 15\n");
}


int main()
{
	int time,status;
	pid_t pid;
	pid=fork();
	
	if(!pid)
	{
		sleep(1);
		signal(0,null_handler);
		kill(getppid(),SIGUSR1);
		exit(0);
	}
	else 
	{
		kill(pid,SIGSTOP);
		signal(SIGUSR1,sigusr1_handler);
		puts("let's play an interesting game");
		puts("what is the answer of this question: 3 * 5 =?");
		scanf("%d",&ans);
		if(ans==15) 
		{
			puts("right answer!");
			kill(pid,SIGKILL);
			wait(NULL);
		}
		else 
		{
			kill(pid,SIGCONT);
			kill(pid,0);
			wait(NULL);
			scanf("%d",&ans);
			if(ans==15) 
			{
				puts("right answer!");
			}
		}
	}
}
