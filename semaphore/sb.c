#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

int freeSeat=3;
sem_t seat,barber,customer;
int who[3],idx=-1;

void* b()
{
	while(1){
	sem_wait(&customer);
	sem_wait(&seat);
	freeSeat++;
	int id=who[idx];
	idx--;
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
		idx++;
		who[idx]=id;
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

