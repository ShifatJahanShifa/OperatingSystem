#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define NUM_OF_THREADS 8

void* rollDice()
{
	int item=rand()%6+1;
	int *result=malloc(sizeof(int));
	*result=item;
	return (void *)result;
}

int main()
{
	pthread_t threads[NUM_OF_THREADS];
	int *ans[NUM_OF_THREADS];
	
	for(int i=0;i<NUM_OF_THREADS;i++)
	{
		pthread_create(&threads[i],NULL,rollDice,NULL);
	}
	for(int i=0;i<NUM_OF_THREADS;i++)
	{
		pthread_join(threads[i],(void **)&ans[i]);
	}
	for(int i=0;i<NUM_OF_THREADS;i++) 
	{
		printf("i am dice %i, my value is %d\n",i+1,*ans[i]);
	}
}
