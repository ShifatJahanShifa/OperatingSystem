#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define NUM_OF_THREAD 5

void *runner(void *arg)
{
	int id=*((int *)arg);
	printf("I'm here %d\n",id);
	printf("my thread id is %d\n",id);
	sleep(3);
	printf("I'm leaving....%d\n",id);
	pthread_exit(NULL);
}


int main()
{
	pthread_t threads[NUM_OF_THREAD];
	int threadId[NUM_OF_THREAD]={1,2,3,4,5};
	int ret;
	
	for(int i=0;i<NUM_OF_THREAD;i++)
	{
		ret=pthread_create(&threads[i],NULL,runner,&threadId[i]);
		if(ret) 
		{
			perror("pthread_create");
			return 1;
		}
	}
	
	for(int i=0;i<NUM_OF_THREAD;i++)
	{
		ret=pthread_join(threads[i],NULL);
		if(ret) 
		{
			perror("pthread_join");
			return 1;
		}
	} 
	
	puts("finally i am main. i am done");
}
	
