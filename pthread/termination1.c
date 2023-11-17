#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

void *start_routine(void *arg) 
{
	char *msg= (char *)arg;
	printf("the message is: %s\n",msg);
	pthread_exit(NULL);
}

int main()
{
	pthread_t thread;
	char msg[20]="hello shifa";
	int ret=pthread_create(&thread,NULL,start_routine,(void *)msg);
	//pthread_join(thread,NULL);
	int unused;
	
	// enabling the cancellation
	ret=pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,&unused);
	if(ret) 
	{
		perror("set cancel state");
		return -1;
	} 
	
	// set cancel type
	ret=pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,&unused);
	if(ret) 
	{
		perror("set cancel type");
		return -1;
	} 
	
	// finally cancel
	ret=pthread_cancel(thread);
	if(ret) 
	{
		perror("cancel");
		return -1;
	} 
	if(ret==0) puts("successfully cancelled");
	
	puts("i am finished finally");
}
