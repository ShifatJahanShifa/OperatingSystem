// description: readers will read but when writers come, then all the writers will finish their work. thus  the readers may starve.

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>

#define READER 5
#define WRITER 3

int cnt=0;
int readCount=0,writeCount=0;
sem_t rmutex,wmutex,readTry,resource;

void* Read(void* arg)
{
	int id=*((int *)arg);
	//while(1)
	//{
		sem_wait(&readTry);
		sem_wait(&rmutex);
		readCount++;
		if(readCount==1) 
			sem_wait(&resource);
		sem_post(&rmutex);
		sem_post(&readTry);
		
		// c s
		printf("I am reading with id %d, value %d\n",id,cnt);
		sleep(3);
		
		sem_wait(&rmutex);
		readCount--;
		if(readCount==0) sem_post(&resource);
		sem_post(&rmutex);
	//}
}

void* Write(void* arg)
{
	int id=*((int *)arg);
	//while(1)
	//{
		sem_wait(&wmutex);
		writeCount++;
		if(writeCount==1) sem_wait(&readTry);
		sem_post(&wmutex);
		
		sem_wait(&resource);
		// cs 
		cnt++;
		printf("i am writer with id %d, value %d\n",id,cnt);
		sleep(3);
		sem_post(&resource);
		
		sem_wait(&wmutex);
		writeCount--;
		if(writeCount==0) sem_post(&readTry);
		sem_post(&wmutex);
	//}
}


int main()
{
	pthread_t readers[READER],writers[WRITER];
	sem_init(&rmutex,0,1);	
	sem_init(&wmutex,0,1);
	sem_init(&readTry,0,1);
	sem_init(&resource,0,1);
	int rids[READER]={1,2,3,4,5},wids[WRITER]={1,2,3};
	
	for(int i=0;i<READER;i++)
	{
		pthread_create(&readers[i],NULL,Read,(void*)&rids[i]);
	}
	for(int i=0;i<WRITER;i++)
	{
		pthread_create(&writers[i],NULL,Write,(void*)&wids[i]);
	}
	
	for(int i=0;i<READER;i++)
	{
		pthread_join(readers[i],NULL);
	}
	for(int i=0;i<WRITER;i++)
	{
		pthread_join(writers[i],NULL);
	}
	
	puts("finally i am main. i am done");
}
