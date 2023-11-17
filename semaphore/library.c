#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>

#define SIZE 10
int shelf[SIZE],in=0,out=0;
sem_t full,empty;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

void* borrow(void* arg)
{
	int id=*((int *)arg);
	while(1)
	{
		sem_wait(&full);
		pthread_mutex_lock(&mutex);
		int item=shelf[out];
		out=(out+1)%SIZE;
		printf("i am borrowing book %d. my id is %d\n",item,id);
		sleep(2);
		pthread_mutex_unlock(&mutex);
		sem_post(&empty);	
	}
}

void* Return(void *arg)
{
	int id=*((int *)arg);
	while(1)
	{
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);
		int item=rand()%15;
		shelf[in]=item;
		in=(in+1)%SIZE;
		printf("i am returning item %d. my id is %d\n",item,id);
		sleep(2);
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
	}
}

int main()
{
	pthread_t bor[6],ret[4];
	sem_init(&full,0,0);
	sem_init(&empty,0,SIZE);
	int bid[]={1,2,3,4,5,6},rid[]={1,2,3,4};
	for(int i=0;i<6;i++)
	{
		pthread_create(&bor[i],NULL,borrow,(void*)&bid[i]);
	}	
	for(int i=0;i<4;i++)
	{
		pthread_create(&ret[i],NULL,Return,(void*)&rid[i]);
	}
	for(int i=0;i<6;i++)
	{
		pthread_join(bor[i],NULL);
	}
	for(int i=0;i<4;i++)
	{
		pthread_join(ret[i],NULL);
	}
}


