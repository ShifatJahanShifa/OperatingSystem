#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>

sem_t deliver,pizza;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
int sliceNum=0;

void* del()
{
	while(1)
	{
		sem_wait(&deliver);
		
		pthread_mutex_lock(&mutex);
		puts("I am making pizza...");
		sleep(2);
		sliceNum=8;
		puts("I am done with making pizza.");
		pthread_mutex_unlock(&mutex);
		for(int i=0;i<8;i++) sem_post(&pizza);
	}
}

void* piz(void* arg)
{
	int id=*((int *)arg);
	while(1)
	{
		sem_wait(&pizza);
		pthread_mutex_lock(&mutex);
		sliceNum--;
		printf("my id %d. i am eating pizza\n",id);
		if(sliceNum==0) 
		{
			sem_post(&deliver);
			printf("my id %d. i am ordering pizza\n",id);
		}  
		pthread_mutex_unlock(&mutex);
		
		// cs
		sleep(2);
	}	
}

int main()
{
	sem_init(&deliver,0,1);
	sem_init(&pizza,0,0);
	pthread_t d,s[5];
	int stu[5]={1,2,3,4,5};
	
	pthread_create(&d,NULL,del,NULL);
	for(int i=0;i<5;i++)
	{
		pthread_create(&s[i],NULL,piz,(void*)&stu[i]);
	}
	pthread_join(d,NULL);
	for(int i=0;i<5;i++)
	{
		pthread_join(s[i],NULL);
	}
	
	
}
