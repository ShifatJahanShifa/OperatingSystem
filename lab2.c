#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<limits.h>
#include<time.h>
#include<sys/mman.h>
#include<fcntl.h>

//#define NUM_OF_PROCESS 10
long long row1,column1,row2,column2;
long long mat1[2000][2000],mat2[2000][2000],resMat[2000][2000];

void createAndInitializeMatrix()
{
	puts("enter the row and column number for the two matrix");
	scanf("%lld%lld%lld%lld",&row1,&column1,&row2,&column2);
	
	for(int i=0;i<row1;i++) 
	{
		for(int j=0;j<column1;j++) 
		{
			mat1[i][j]=rand()%50;
		} 
	} 
	for(int i=0;i<row2;i++) 
	{
		for(int j=0;j<column2;j++) 
		{
			mat2[i][j]=rand()%50;
		} 
	} 
}  

void displayMatrix()
{
	puts("first matrix");
	for(int i=0;i<row1;i++) 
	{
		for(int j=0;j<column1;j++) 
		{
			printf("%lld ",mat1[i][j]); 
		} 
		puts("");
	} 
	puts("\nsecond matrix");
	for(int i=0;i<row2;i++) 
	{
		for(int j=0;j<column2;j++) 
		{
			printf("%lld ",mat2[i][j]); 
		} 
		puts("");
	} 
}

void matrixMultiply(int startRow,int endRow)
{
	for(int i=startRow;i<endRow;i++) 
	{
		for(int j=0;j<column2;j++) 
		{
			resMat[i][j]=0;
			for(int k=0;k<column1;k++)
			{
				resMat[i][j]+=(mat1[i][k]*mat2[k][j]); 
			}
		}
	}
}

int main()
{ 
	createAndInitializeMatrix();
	displayMatrix();	
	int NUM_OF_PROCESS=row1;
	if(column1!=row2) 
	{
		fprintf(stderr,"the two matrix can't be multiplied\n");
		return 1;
	} 
	
	int shmId=shm_open("/sharedMatrix",O_CREAT | O_RDWR, 0666);
	if (shmId == -1) 
	{
        	perror("shm_open failed");
        	return 1;
    	}
    	if (ftruncate(shmId, sizeof(long long int) * row1 * column2) == -1) 
    	{
        	perror("ftruncate failed");
        	return 1;
    	}
	
	int *shmPtr=(int *)mmap(0, sizeof(long long int)*row1*column2, PROT_WRITE, MAP_SHARED, shmId,0);
	if (shmPtr == MAP_FAILED) 
	{
        	perror("mmap failed");
        	return 1;
  	}

	int process[NUM_OF_PROCESS];
	int rowPerChild=row1/NUM_OF_PROCESS;
	long long startRow=0,endRow=0;
	int pid;
	clock_t startTime = clock();
	for(int i=0;i<NUM_OF_PROCESS;i++)
	{
		startRow=(row1 / NUM_OF_PROCESS)*i;
		endRow=(row1 / NUM_OF_PROCESS)*(i+1);
		
		if((pid=fork())==0) 
		{
			matrixMultiply(startRow, endRow);
                	for (int j = startRow; j < endRow; j++) 
                	{	
                		for (int k = 0; k < column2; k++) 
                		{
                    			shmPtr[j * column2 + k] = resMat[j][k];
                		}
            		}
			exit(0);
		} 
		else if(pid<0)
		{
			fprintf(stderr,"fork failed");
			return 1; 
		}
	}
	
	for(int i=0;i<NUM_OF_PROCESS;i++)
	{
		//wait(NULL);
		int status;
       	 	if (wait(&status) == -1) 
       	 	{
            		perror("wait failed"); // Print an error message for wait failure
            		return 1;
        	}
	}
	clock_t endTime = clock(); 
	double executionTimeParallel=(double)(endTime - startTime) / CLOCKS_PER_SEC;
	    
    	for (int i = 0; i < row1; i++) 
    	{
        	for (int j = 0; j < column2; j++) 
        	{
            		resMat[i][j] = shmPtr[i * column2 + j];
            	}
        }
        //shm_unlink("/sharedMatrix");
        if (shm_unlink("/sharedMatrix") == -1) 
        {
        	perror("shm_unlink failed");
        	return 1;
    	}
	
	// parent process
	startTime=clock();
        for (int i = 0; i < row1; i++) 
        {
        	for (int j = 0; j < column2; j++) 
        	{
            		resMat[i][j] = 0;
            		for (int k = 0; k < column1; k++) 
            		{
                		resMat[i][j] += mat1[i][k] * mat2[k][j];
            		}
        	}
    	}
    	endTime=clock();
    
    	double executionTimeSequentially = (double)(endTime - startTime) / CLOCKS_PER_SEC;
    	
    	puts("the resultant matrix");
    	for (int i = 0; i < row1; i++) 
    	{
         	for (int j = 0; j < column2; j++) 
         	{
             		printf("%9lld ", resMat[i][j]);
         	}
        	puts("\n");
     	}
	
	printf("parallel time: %.6f\n",executionTimeParallel);
	printf("sequential time: %.6f\n",executionTimeSequentially);
    
}
