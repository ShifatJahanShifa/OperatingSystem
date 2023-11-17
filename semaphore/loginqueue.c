// problem description: there is a login queue where 12 people can receive service at a time. suppose there are total 16 people in a queue. then print the sequence of getting service without occuring deadlock.

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#define THREADS_NUM 16

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
sem_t semaphore;

void* getService(void *arg)
{
	int id=*((int *)arg);
	printf("i'm waiting in the queue...%d\n",id);
	sem_wait(&semaphore);
	printf("my id is %d. i'm getting service.\n",id);
	sleep(1);
	printf("my id is %d. i'm leaving service.\n",id);
	sem_post(&semaphore);	
}


int main()
{
	pthread_t threads[THREADS_NUM];
	int idx[THREADS_NUM];
	sem_init(&semaphore,0,12);
	for(int i=0;i<THREADS_NUM;i++)
	{
		idx[i]=i;
		pthread_create(&threads[i],NULL,getService,(void*)&idx[i]);
	}	
	for(int i=0;i<THREADS_NUM;i++)
	{
		pthread_join(threads[i],NULL);
	}	
}
