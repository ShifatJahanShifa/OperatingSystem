// problem description: there are two threads. one thread is car, another thread is fueler. scenario is the fueler fuels the tank for consecutive five times by 15 litre. the car wants 40 litre fuel. initially the tank has 0 litre fuel. finally i am able to insert emoji😍️


#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

int fuel=0;
pthread_mutex_t mutexFuel=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condFuel=PTHREAD_COND_INITIALIZER;

void* car()
{
	pthread_mutex_lock(&mutexFuel);
	while(fuel<40) 
	{
		puts("not enough fuel. waiting....");
		pthread_cond_wait(&condFuel,&mutexFuel);
	}
	fuel-=40;
	pthread_mutex_unlock(&mutexFuel);
	puts("i am done");
}

void* fueler()
{
	for(int i=0;i<5;i++) 
	{
		pthread_mutex_lock(&mutexFuel);
		fuel+=15;
		puts("i have added fuel");
		pthread_mutex_unlock(&mutexFuel);	
		pthread_cond_signal(&condFuel);
		sleep(1);
	}	
}


int main()
{
	pthread_t thread1,thread2;
	
	pthread_create(&thread1,NULL,car,NULL);
	pthread_create(&thread2,NULL,fueler,NULL);
	
	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);
	
}
	
