// problem description: producer-consumer problem. the buffer is bounded, has fixed size 10. there is one producer and consumer. when the buffer is empty the consumer cannot consume any item. it waits the buffer to be fulled by at least 1 item. when the buffer is full and there is no empty space in the buffer, the producer cannot produce any item. so it will wait for the consumer to consume an item. the major assumption here is that the scanning in the buffer is circular rotation and it is true for both producer and consumer. well, what if the producing and consuming on demand based?? i haven't tried it before.

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

#define BUFFER_SIZE 10
#define THREAD_NUM 8

int buffer[BUFFER_SIZE];
int in=0,out=0;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
sem_t semFull,semEmpty;

void* produce()
{
	for(;;)
	{
		int item=rand()%20;	
		sem_wait(&semEmpty);
		pthread_mutex_lock(&mutex);
		buffer[in]=item;
		in=(in+1)%BUFFER_SIZE;
		printf("produced item %d\n",item);
		pthread_mutex_unlock(&mutex);
		sem_post(&semFull);
		
		sleep(1);
	}
}

void* consume()
{
	for(;;)
	{
		sem_wait(&semFull);
		pthread_mutex_lock(&mutex);
		int item=buffer[out];
		out=(out+1)%BUFFER_SIZE;
		printf("consumed item %d\n",item);
		pthread_mutex_unlock(&mutex);
		sem_post(&semEmpty);
		//sleep(1);
	}
}


int main()
{
	pthread_t threads[THREAD_NUM];
	sem_init(&semFull,0,0);
	sem_init(&semEmpty,0,BUFFER_SIZE);
	
	for(int i=0;i<THREAD_NUM;i++)
	{
		if(!(i&1))
		pthread_create(&threads[i],NULL,produce,NULL);
		else 
		pthread_create(&threads[i],NULL,consume,NULL);
	}
	for(int i=0;i<THREAD_NUM;i++)
	{
		pthread_join(threads[i],NULL);
	}
}
