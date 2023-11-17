// description: sleeping barber problem in fifo manner

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

int freeSeat=3;
sem_t seat,barber,customer;
int front=0,rear=0,queue[3];

void enqueue(int id)
{
	queue[rear]=id;
	rear=(rear+1)%3;
}

int dequeue()
{
	int id=queue[front];
	front=(front+1)%3;
	return id;
}

void* b()
{
	while(1){
	sem_wait(&customer);
	sem_wait(&seat);
	freeSeat++;
	int id=dequeue();
	printf("b freeseat %d\n",freeSeat);
	sem_post(&barber);
	sem_post(&seat);
	
	// cs
	printf("I am barber. I am cutting hair %d\n",id);
	sleep(3);	
	}
}

void* c(void* arg)
{
	int id=*((int *)arg);
	while(1){
	sem_wait(&seat);
	if(freeSeat>0)
	{
		freeSeat--;
		enqueue(id);
		printf("my id %d. i am waiting for barber.\n",id);
		printf("c freeseat %d\n",freeSeat);
		sem_post(&customer);
		sem_post(&seat);
		sem_wait(&barber);
		
		// cs
		printf("my id is  %d. i am cutting hair\n",id);
	}
	else sem_post(&seat);
	}
}

int main()
{
	pthread_t Barber,Customer[6];
	sem_init(&seat,0,1);
	sem_init(&barber,0,0);
	sem_init(&customer,0,0);
	int cid[6]={1,2,3,4,5,6};
	
	pthread_create(&Barber,NULL,b,NULL);
	for(int i=0;i<6;i++)
	{
		pthread_create(&Customer[i],NULL,c,(void*)&cid[i]);
	}
	
	pthread_join(Barber,NULL);
	for(int i=0;i<6;i++)
	{
		pthread_join(Customer[i],NULL);
	}
	
	puts("i am main, finally i am done");
}

