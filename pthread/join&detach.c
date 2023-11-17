#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

void *start_routine(void *)
{
	puts("here i am");
	pthread_exit(NULL);
}

int main() 
{
	pthread_t threadToJoin,threadToDetach;
	int ret;
	ret=pthread_create(&threadToJoin,NULL,start_routine,NULL);
	ret=pthread_create(&threadToDetach,NULL,start_routine,NULL);
	
	ret=pthread_join(threadToJoin,NULL);
	if(!ret) puts("joined successfully");
	
	ret=pthread_detach(threadToDetach);
	if(!ret) puts("detached successfully");
	
	puts("i am finished finally");
}
