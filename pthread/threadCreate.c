#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

void *start_routine(void *arg) 
{
	char *msg= (char *)arg;
	puts("starting");
	printf("the message is: %s\n",msg);
	sleep(3);     // achieved parallellism is shown by this statement
	puts("ending");
	pthread_exit(NULL);
}

int main()
{
	pthread_t t1,t2;
	char msg[20]="hello shifa";
	int ret=pthread_create(&t1,NULL,start_routine,(void *)msg);
	ret=pthread_create(&t2,NULL,start_routine,(void *)msg);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	puts("i am finished finally");
}
