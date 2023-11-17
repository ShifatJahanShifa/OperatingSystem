// description: i want to see two functions: execl and execv. there will be four processes. 1. only show that it is parent, 2.run ps command, 3. run ls command, 4. run vi command

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>

int main()
{
	//printf("before");
	//execl("/bin/vi","vi","/home/shifa/Pictures/Screenshots/blockElements.png",NULL);
	//printf("after");
	pid_t pid1,pid2;
	pid1=fork();
	pid2=fork();
	
	// child=1
	if(pid1==0 && pid2!=0) 
	{
		sleep(5);
		puts("i am child1");
		printf("my pid: %d\n",getpid());
		printf("my parent id: %d\n",getppid());
		execl("/bin/vi","vi",NULL);
	}
	if(pid1!=0 && pid2==0) 
	{
		sleep(3);
		puts("i am child2");
		printf("my pid: %d\n",getpid());
		printf("my parent id: %d\n",getppid());
		char *args[]={"ls","-l",NULL};
		//execl("/bin/ls","ls","-l",NULL);
		execv("/bin/ls",args);
	}
	if(pid1==0 && pid2==0) 
	{
		puts("i am grand child");
		printf("my pid: %d\n",getpid());
		printf("my parent id: %d\n",getppid());
		execl("/bin/ps","ps",NULL);
	}
	if(pid1!=0 && pid2!=0)
	{
		puts("i am parent🥰️");
		printf("my pid: %d\n",getpid());
	}
	
	while(wait(NULL)!=-1 || errno!=ECHILD) ;
	puts("finally i am done");
	
}
