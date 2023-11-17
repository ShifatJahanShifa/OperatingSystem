#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<limits.h>
#include<time.h>

long long row1,column1,row2,column2;
long long **mat1, **mat2, **resMat;

void createAndInitializeMatrix()
{
	puts("enter the row and column number for the two matrix");
	scanf("%lld%lld%lld%lld",&row1,&column1,&row2,&column2);
	long long int v=1;
	
	mat1 = (long long **)malloc(row1 * sizeof(long long *));
    	for (int i = 0; i < row1; i++)
    	{
        	mat1[i] = (long long *)malloc(column1 * sizeof(long long));
   	}

    	mat2 = (long long **)malloc(row2 * sizeof(long long *));
    	for (int i = 0; i < row2; i++) 
    	{
        	mat2[i] = (long long *)malloc(column2 * sizeof(long long));
   	}

	for(int i=0;i<row1;i++) 
	{
		for(int j=0;j<column1;j++) 
		{
			mat1[i][j]=v%30;
			v++; 
		} 
	} 
	v=1;
	for(int i=0;i<row2;i++) 
	{
		for(int j=0;j<column2;j++) 
		{
			mat2[i][j]=v%30;
			v++; 
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


int slave(int fdValue,long long int startRow,long long int endRow)
{
	for(int i=startRow;i<endRow;i++) 
	{
		for(int j=0;j<column2;j++) 
		{
			for(int k=0;k<column1;k++)
			{
				resMat[i][j]+=(mat1[i][k]*mat2[k][j]); 
			}
		} 
	} 
	
        size_t bytesWrite=write(fdValue,&resMat[startRow][0],sizeof(long long)*(endRow-startRow)*column2);
	if(bytesWrite==-1) 
	{
		perror("failed to write");
		return 1;
	}

	if (close(fdValue) == -1)
	{
        	perror("close failed for write end of pipe of the slave process"); // Error message for close call
        	return 1;
        }
     
        return 0;
}

	
int main()
{
	createAndInitializeMatrix();
	displayMatrix();
	
	resMat = (long long **)malloc(row1 * sizeof(long long *));
    	for (int i = 0; i < row1; i++) 
    	{
        	resMat[i] = (long long *)malloc(column2 * sizeof(long long));
    	}
	for(int i=0;i<row1;i++) 
	{
		for(int j=0;j<column2;j++) 
		{
			resMat[i][j]=0;
		} 
	} 
	
	if(column1!=row2) 
	{
		fprintf(stderr,"the two matrix can't be multiplied\n");
		return 1;
	} 
	
	int fd[2];
	
	// create pipe in the master process
	if(pipe(fd)==-1) 
	{
		fprintf(stderr,"pipe failed");
		return 1;
	} 
	
	
	int totalChild=4;
	int rowPerChild=row1/totalChild;
	long long startRow=0,endRow=0;
	int pid;
	int rem;
		
	clock_t startTime = clock();
	for(int i=0;i<totalChild;i++)
	{
		//startRow=i;
		//endRow=startRow+1;
		startRow=(row1 / totalChild)*i;
		endRow=(row1 / totalChild)*(i+1);
		if((pid=fork())==0) 
		{
			close(fd[0]);   // closing the read 
			if(slave(fd[1],startRow,endRow))
			{
				fprintf(stderr,"failure in the slave process");
				return 1;
			}
			exit(0);
		} 
		else if(pid<0)
		{
			fprintf(stderr,"fork failed");
			return 1; 
		}
		
	}
	
	for(int i=0;i<totalChild;i++)
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
	
	if (close(fd[1]) == -1)
	{
        	perror("close failed for write end of pipe of the master process"); // Error message for close call
        	return 1;
        }

	// collecting and aggregating result
	for(int i=0;i<totalChild;i++)
	{
		//startRow=i;
		//endRow=i+1;
		startRow=(row1 / totalChild)*i;
		endRow=(row1 / totalChild)*(i+1);
		size_t numElements = (size_t)(endRow - startRow) * column2;
		size_t bytesRead=read(fd[0],&resMat[startRow][0],sizeof(long long)*numElements);
		if (bytesRead == -1)
		{
                	perror("read failed"); // Print an error message
            		return 1;
        	}
	}
	
	

    	// Calculate the execution time in seconds
    	double executionTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
	
	if (close(fd[0]) == -1)
	{
        	perror("close failed for read end of pipe of the master process"); // Error message for close call
        	return 1;
        }
	
	// parent process
	startTime = clock();
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
	endTime = clock();
	
	// showing the output
	puts("\nthe resultant matrix:");
	for(int i=0;i<row1;i++) 
	{
		for(int j=0;j<column2;j++) 
		{
			printf("%lld ",resMat[i][j]); 
		} 
		puts("");
	} 
	printf("\nExecution Time parallelly: %.6f seconds\n", executionTime);
	executionTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
	printf("\nExecution Time Sequentially: %.6f seconds\n", executionTime);
	
	for (int i = 0; i < row1; i++) 
	{
        	free(mat1[i]);
    	}
    	free(mat1);

    	for (int i = 0; i < row2; i++) 
    	{
        	free(mat2[i]);
    	}
    	free(mat2);

    	for (int i = 0; i < row1; i++) 
    	{
        	free(resMat[i]);
    	}
    	free(resMat);
}
	
	
