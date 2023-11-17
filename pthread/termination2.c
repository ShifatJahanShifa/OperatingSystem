#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

void *start_routine1(void *arg) 
{
	pthread_t thread = *((pthread_t *)arg);
	printf("i am going to cancel thread2\n");
	int ret;
	int unused;
	
	// enabling the cancellation
	ret=pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,&unused);
	if(ret) 
	{
		perror("set cancel state");
		//return -1;
	} 
	
	// set cancel type
	ret=pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,&unused);
	if(ret) 
	{
		perror("set cancel type");
		//return -1;
	} 
	
	// finally cancel
	ret=pthread_cancel(thread);
	if(ret) 
	{
		perror("cancel");
		//return -1;
	} 
	if(ret==0) puts("successfully cancelled");
	pthread_exit(NULL);
}

void *start_routine2(void *arg) 
{
	char *msg= (char *)arg;
	printf("the message is: %s\n",msg);
	printf("and my TID is: %ld\n",pthread_self());
	pthread_exit(NULL);
}

int main()
{
	pthread_t thread1,thread2;
	char msg2[30]="this is your road...";
	int ret;
	ret=pthread_create(&thread1,NULL,start_routine1,&thread2);
	if(ret) 
	{
		perror("pthread_create");
		return -1;
	}
	ret=pthread_create(&thread2,NULL,start_routine2,(void*)msg2);
	if(ret) 
	{
		perror("pthread_create");
		return -1;
	}
	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);
	
	puts("i am finished finally");
}
