#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main () {
    
    clock_t start = clock();

    int rows1, col1, rows2, col2;

    printf("continuous multiplication\nenter rows1, columns1, columns2:\n");
	scanf ("%i %i %i", &rows1, &col1, &col2);
	rows2 = col1;

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

    printf("\nresult C:\n");
    for (int i = 0; i < rows1; i++) 
    {
    	for (int j = 0; j < col2; j++) 
    		{
    			C[i][j] = 0;
        		for(int k = 0; k < col1; k++)
        		{
        		C[i][j] += A[i][k] * B[k][j];
    			}
    			printf("%i ", C[i][j]);
    		}
    	printf("\n");
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
    
    clock_t end = clock();
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;
	printf("\n%s%f\n", "time: ", seconds);

	return 0;
}