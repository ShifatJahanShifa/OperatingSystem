#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
	pid_t pid=getpid();
	printf("process with pid: %d\n",pid);
	raise(SIGUSR2);
	sleep(2);
}
