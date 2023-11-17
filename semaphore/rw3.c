#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>

#define READER 5
#define WRITER 5

sem_t resource,rmutex,serviceQueue;
int readcount=0;

void* reads(void* arg) 
{
	int id=*((int *)arg);
	
	printf("I am waiting for reading. my id is %d\n",id);
	// mutex lock
	sem_wait(&serviceQueue);
	sem_wait(&rmutex);
	readcount++;
	if(readcount==1) sem_wait(&resource); 
	sem_post(&serviceQueue);
	sem_post(&rmutex);
	
	// critical section
	printf("I am reading. my id is %d\n",id);
	sleep(4);
	
	// leaving
	sem_wait(&rmutex);
	readcount--;
	if(readcount==0) sem_post(&resource); 
	sem_post(&rmutex);
	printf("I am leaving. my id is %d\n",id);
} 

void* writes(void* arg)
{
	int id=*((int *)arg);
	printf("I am waiting for writing. my id is %d\n",id);
	sem_wait(&serviceQueue);
	sem_wait(&resource);
	printf("I have got resource. my id is %d\n",id);
	sem_post(&serviceQueue);
	printf("I am writing. my id is %d\n",id);
	sleep(3);
	sem_post(&resource);
	printf("I am done with writing. my id is %d\n",id);
}

int main()
{
	pthread_t readers[READER],writers[WRITER];
	sem_init(&resource,0,1);
	sem_init(&rmutex,0,1);
	sem_init(&serviceQueue,0,1); 
	int readerID[READER],writerID[WRITER];
	// pthreads creation
	for(int i=0;i<READER;i++) 
	{
		readerID[i]=i;
		pthread_create(&readers[i],NULL,reads,(void *)&readerID[i]);
		
		
	}
	for(int i=0;i<WRITER;i++) 
	{
		writerID[i]=i;
		pthread_create(&writers[i],NULL,writes,(void *)&writerID[i]);
	}
	
	for(int i=0;i<READER;i++) 
	{
		pthread_join(readers[i],NULL);
	}
	for(int i=0;i<WRITER;i++) 
	{
		pthread_join(writers[i],NULL);
	}
	
	puts("finally i'm done"); 
}
	
