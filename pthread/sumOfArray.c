// problem description: sum of array using two threads


#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

int primes[10]={2,3,5,7,11,13,17,19,23,29};

void* sumA(void *arg)
{
	int i= *((int*)arg);
	int ans=0;
	int *ret=malloc(sizeof(int));
	int limit=i+5;
	for(;i<limit;i++) 
	{
		ans+=primes[i];
	}

	*ret=ans;
	return ret;
}


int main()
{
	pthread_t threads[2];
	int idx[2];
	int sum=0;
	for(int i=0;i<2;i++) 
	{
		idx[i]=i*5;
		pthread_create(&threads[i],NULL,sumA,(void*) &idx[i]);
	}
	for(int i=0;i<2;i++) 
	{
		int *res;
		pthread_join(threads[i],(void **) &res);
		sum+=*res;
	}
	printf("the sum is: %d\n",sum);
}
