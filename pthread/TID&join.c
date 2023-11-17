#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

void *start_routine1(void *arg) 
{
	char *msg= (char *)arg;
	printf("the message is: %s\n",msg);
	printf("my TID is: %ld\n",pthread_self());
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
	char msg1[20]="hello shifa";
	char msg2[30]="this is your road...";
	int ret;
	ret=pthread_create(&thread1,NULL,start_routine1,(void*)msg1);
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
	
	ret=pthread_equal(thread1,thread2);
	ret ? puts("equal") : puts("not equal");
	puts("i am finished finally");
}
