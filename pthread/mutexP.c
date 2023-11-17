// description: meet race condition first, then apply mutex. also work with for loop for multiple threads


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define NUM_OF_THREADS 5
long long int ans=0;

pthread_mutex_t mutex;

void* increment()
{
	pthread_mutex_lock(&mutex);
	for(int i=1;i<=1000000;i++)
		ans++;
	pthread_mutex_unlock(&mutex);
}


int main()
{
	pthread_t threads[NUM_OF_THREADS];
	pthread_mutex_init(&mutex,NULL);
	for(int i=0;i<NUM_OF_THREADS;i++)
	{
		pthread_create(&threads[i],NULL,increment,NULL);
	}
	for(int i=0;i<NUM_OF_THREADS;i++)
	{
		pthread_join(threads[i],NULL);
	}
	
	puts("I'm main thread!");
	printf("the value is: %lld\n",ans);
}
	
