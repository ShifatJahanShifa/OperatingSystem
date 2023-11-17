#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>

long long row1,column1,row2,column2;
long long mat1[1000][1000],mat2[1000][1000];
long long resMat[1000][1000];

void createAndInitializeMatrix()
{
	puts("enter the row and column number for the two matrix");
	scanf("%lld%lld%lld%lld",&row1,&column1,&row2,&column2);
	long long int v=1;
	for(int i=0;i<row1;i++) 
	{
		for(int j=0;j<column1;j++) 
		{
			mat1[i][j]=v;
			v++; 
		} 
	} 
	v=1;
	for(int i=0;i<row2;i++) 
	{
		for(int j=0;j<column2;j++) 
		{
			mat2[i][j]=v;
			v++; 
		} 
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
			printf(" here %lld\n",resMat[i][j]);
		} 
	} 
			
	size_t bytesWrite=write(fdValue,&resMat[startRow][0],sizeof(long long)*(endRow-startRow)*column2);
	printf("slave %lld\n",startRow);
	if(bytesWrite==-1) 
	{
		perror("failed to write");
		return 1;
	}
	//close(fdValue);
	if (close(fdValue) == -1)
	{
        	perror("close failed for write end of pipe of the slave process"); // Error message for close call
        	return 1;
        }
       // printf("slave %d\n",startRow);
        return 0;
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
	puts("second matrix");
	for(int i=0;i<row2;i++) 
	{
		for(int j=0;j<column2;j++) 
		{
			printf("%lld ",mat2[i][j]); 
		} 
		puts("");
	} 
}
	
int main()
{
	createAndInitializeMatrix();
	displayMatrix();
	//int finalMat[row1][column2];
	for(int i=0;i<row1;i++) 
	{
		for(int j=0;j<column2;j++) 
		{
			resMat[i][j]=0;
		} 
	} 
	
	// main task from here
	if(column1!=row2) 
	{
		fprintf(stderr,"the two matrix can't be multiplied");
		return 1;
	} 
	
	int fd[2];
	// create pipe
	if(pipe(fd)==-1) 
	{
		fprintf(stderr,"pipe failed");
		return 1;
	} 
	
	int totalChild=row1;
	int rowPerChild=row1/totalChild;
	long long startRow=0,endRow=0;
	int pid;
	for(int i=0;i<totalChild;i++)
	{
		startRow=i;
		endRow=startRow+1;
		
		printf("%lld\n",startRow);
		if((pid=fork())==0) 
		{
			close(fd[0]);
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
	//close(fd[1]);
	if (close(fd[1]) == -1)
	{
        	perror("close failed for write end of pipe of the master process"); // Error message for close call
        	return 1;
        }

	// collecting result
	
	
	for(int i=0;i<totalChild;i++)
	{
		startRow=i;
		endRow=i+1;
		
		size_t bytesRead=read(fd[0],&resMat[startRow][0],sizeof(long long)*(endRow-startRow)*column2);
		if (bytesRead == -1)
		{
                	perror("read failed"); // Print an error message
            		return 1;
        	}
	}
	//close(fd[0]);
	if (close(fd[0]) == -1)
	{
        	perror("close failed for read end of pipe of the master process"); // Error message for close call
        	return 1;
        }
	
	for(int i=0;i<row1;i++) 
	{
		for(int j=0;j<column2;j++) 
		{
			printf("%lld ",resMat[i][j]); 
		} 
		puts("");
	} 
}
	
	
