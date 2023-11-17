#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>

#define READER 5
#define WRITER 5

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
sem_t Write;
int readcount=0;

void* reads(void* arg) 
{
	int id=*((int *)arg);
	
	printf("I am waiting for reading. my id is %d\n",id);
	// mutex lock
	pthread_mutex_lock(&mutex);
	readcount++;
	if(readcount==1) sem_wait(&Write); 
	pthread_mutex_unlock(&mutex);
	
	// critical section
	printf("I am reading. my id is %d\n",id);
	sleep(4);
	
	// leaving
	pthread_mutex_lock(&mutex);
	readcount--;
	if(readcount==0) sem_post(&Write); 
	pthread_mutex_unlock(&mutex);
	printf("I am leaving. my id is %d\n",id);
} 

void* writes(void* arg)
{
	int id=*((int *)arg);
	printf("I am waiting for writing. my id is %d\n",id);
	sem_wait(&Write);
	printf("I am writing. my id is %d\n",id);
	sleep(3);
	sem_post(&Write);
	printf("I am done with writing. my id is %d\n",id);
}

int main()
{
	pthread_t readers[READER],writers[WRITER];
	sem_init(&Write,0,1);
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
	
