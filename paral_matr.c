#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


#define procNum 3


int main() {

	clock_t start = clock();

    int rows1, col1, rows2, col2;
    int i;    
    int *C1;
    int shmid;

    printf("parallel multiplication\nenter rows1, columns1, columns2:\n");
	scanf ("%i %i %i", &rows1, &col1, &col2);
	rows2 = col1;

	shmid = shmget (IPC_PRIVATE, sizeof(int) * (rows1 * col2), 0666 | IPC_CREAT);
	C1 = (int*)shmat(shmid, 0, 0);

	int **A = (int**)malloc(rows1 * sizeof(int*));
	int **B = (int**)malloc(rows2 * sizeof(int*));
	int **C = (int**)malloc(rows1 * sizeof(int*));

	for (int i = 0; i < rows1; i++)
	{
    	A[i] = (int*)malloc(col1 * sizeof(int));
	}
	for (int i = 0; i < rows2; i++)
	{
    	B[i] = (int*)malloc(col2 * sizeof(int));
	}	
	for (int i = 0; i < rows1; i++)
	{
    	C[i] = (int*)malloc(col2 * sizeof(int));
	}

	srand(time(NULL));

	printf("\nmatrices A, B:\n");
	for (int i = 0; i < rows1; i++) 
	{
    	for (int j = 0; j < col1; j++)
    	{
        	A[i][j] = (rand() % 10) + 1;
        	printf("%i ", A[i][j]);
    	}
    	printf("\n");
    }
    printf("\n");
	for (int i = 0; i < rows2; i++)
	{
    	for (int j = 0; j < col2; j++)
    	{
        	B[i][j] = (rand() % 10) + 1;
    		printf("%i ", B[i][j]);
     	}
    	printf("\n");
    }
    printf("\n");
    int count;
    int pid = 0;
    for (i = 0; i < procNum; i++) {
    	pid = fork();
        if(pid < 0) {
            printf("fork_error");
        }
    	if (pid == 0) {
            break;
    	}
    }

label:    if (pid == 0) {
            for (int k = 0; k < col2; k++) {
                for (int l = 0; l < col1; l++) {
                    C1[i*col1+k] += A[i][l] * B[l][k];
                }
            }
            if (i + procNum <= rows1) {
                i = i + procNum;
                goto label;
            }
            exit(0);
    }

    for(i = 0; i < rows1; i++) {
        int status = 0;
        pid_t child_pid = wait(&status);
    }

    printf("matrix C:\n");    
    

    for (int k = 0; k < (rows1 * col2); k++) {
                if (k%col2 == 0) {
                    printf("\n");
                }
                printf("%i ", C1[k]);
    }

    for (int i = 0; i < rows1; i++) {
        free(A[i]);
        free(C[i]);
    }
    for (int i = 0; i < rows2; i++) {
        free(B[i]);
    }
    free(A);
    free(B);
    free(C);
    shmdt(C1);   
    shmctl(shmid, IPC_RMID, NULL);
    clock_t end = clock();
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;
	printf("\n%s%f\n", "time: ", seconds);
	return 0;
}