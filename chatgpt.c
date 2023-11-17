#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include<time.h>

#define N 100  // Adjust the matrix size as needed    
#define NUM_PROCESSES 4

int A[N][N];
int B[N][N];
int C[N][N];

void generateMatrix(int matrix[N][N]) 
{
    for (int i = 0; i < N; i++) 
    {
        for (int j = 0; j < N; j++) 
        {
            matrix[i][j] = rand() % 20;
        }
    }
}

void printMatrix(int matrix[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void matrixMultiply(int start, int end) {
    for (int i = start; i < end; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    generateMatrix(A);
    generateMatrix(B);

    printf("Matrix A:\n");
    // Comment out the following line when working with large matrices
    printMatrix(A);

    printf("Matrix B:\n");
    // Comment out the following line when working with large matrices
    printMatrix(B);

    int shm_id = shm_open("/shared_matrix", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_id, sizeof(int) * N * N);

    int *shm_ptr = (int *)mmap(0, sizeof(int) * N * N, PROT_WRITE, MAP_SHARED, shm_id, 0);

    int processes[NUM_PROCESSES];
    clock_t startTime = clock();
    for (int i = 0; i < NUM_PROCESSES; i++) 
    {
        int start = (N / NUM_PROCESSES) * i;
        int end = (N / NUM_PROCESSES) * (i + 1);
        processes[i] = fork();

        if (processes[i] == 0) {  // Child process
            matrixMultiply(start, end);

            for (int j = start; j < end; j++) {
                for (int k = 0; k < N; k++) {
                    shm_ptr[j * N + k] = C[j][k];
                }
            }
            exit(0);
        } else if (processes[i] < 0) {
            perror("Fork failed");
            return 1;
        }
    }

    for (int i = 0; i < NUM_PROCESSES; i++) {
        waitpid(processes[i], NULL, 0);
    }
    clock_t endTime = clock(); 
    double executionTimeS = (double)(endTime - startTime) / CLOCKS_PER_SEC;
    
    shm_unlink("/shared_matrix");
    printf("Resultant Matrix C:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = shm_ptr[i * N + j];
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
    // Comment out the following lines when working with large matrices
    // for (int i = 0; i < N; i++) {
    //     for (int j = 0; j < N; j++) {
    //         printf("%d ", C[i][j]);
    //     }
    //     printf("\n");
    // }
    
    // parent
    int temp[N][N];
    //startTime=clock();
    for(int i=0;i<N;i++) 
    {
    	for(int j=0;j<N;j++) 
    		temp[i][j]=0;
    }
    startTime=clock();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    
    endTime=clock();
    
    double executionTimeP = (double)(endTime - startTime) / CLOCKS_PER_SEC;
    puts("again");
    for (int i = 0; i < N; i++) {
         for (int j = 0; j < N; j++) {
             printf("%d ", C[i][j]);
         }
         printf("\n");
     }
	printf("parallel time: %.6f\n",executionTimeS);
	printf("sequential time: %.6f\n",executionTimeP);
    return 0;
}

