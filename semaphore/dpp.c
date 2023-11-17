#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>

#define PHIL 5
#define CHOPSTICK 5

sem_t chpstk[CHOPSTICK];

void think(int id)
{
	printf("i'm philosopher with id %d. i am thinking\n",id);
	sleep(2);
}

void takeCS(int id)
{
	int left,right;
	left=id;
	right=(id+1)%CHOPSTICK;
	
	printf("i am phil. %d.i am taking chopstick.\n",id);
	if(id&1) 
	{
		// left
		sem_wait(&chpstk[left]);
		printf("id %d, taking left stick %d\n",id,left);
		sem_wait(&chpstk[right]);
		printf("id %d, taking right stick %d\n",id,right);
	}
	else 
	{
		sem_wait(&chpstk[right]);
		printf("id %d, taking right stick %d\n",id,right);
		sem_wait(&chpstk[left]);
		printf("id %d, taking left stick %d\n",id,left);
	}
} 

void eat(int id)
{
	printf("i am phil. %d. now i am eating\n",id);
	sleep(3);
}

void putCS(int id)
{
	int left,right;
	left=id;
	right=(id+1)%CHOPSTICK;
	
	printf("i am phil. %d.i am putting down chopstick.\n",id);
	if(id&1) 
	{
		// left
		sem_post(&chpstk[left]);
		printf("id %d, puttingleft stick %d\n",id,left);
		sem_post(&chpstk[right]);
		printf("id %d, putting right stick %d\n",id,right);
	}
	else 
	{
		sem_post(&chpstk[right]);
		printf("id %d, putting right stick %d\n",id,right);
		sem_post(&chpstk[left]);
		printf("id %d, putting left stick %d\n",id,left);
	}
} 

void* philo(void *arg)
{
	for(;;){
	int id=*((int*)arg);
	think(id);
	takeCS(id);
	eat(id);
	putCS(id);}
}

int main()
{
	pthread_t philosopher[PHIL];
	for(int i=0;i<CHOPSTICK;i++) 
	sem_init(&chpstk[i],0,1);
	
	int philID[PHIL],chsID[CHOPSTICK]={0,1,2,3,4};
	// pthreads creation
	for(int i=0;i<PHIL;i++) 
	{
		philID[i]=i;
		pthread_create(&philosopher[i],NULL,philo,(void *)&philID[i]);
		
		
	}
	for(int i=0;i<PHIL;i++) 
	{
		pthread_join(philosopher[i],NULL);
	}
	
	puts("finally i'm done"); 
}
	
