#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>


#define PROC_NUM 2

double function(double A, double B, double C, double x);

int main() {

	clock_t start = clock();

    double left_x, right_x;
    int A = 1;
    int B = 0;
    int C = -4;
	double *res;
	int shmid;
	int i;
    int steps;
    printf("parallel evaluation of integral using forks\nenter left x, right x, amount of steps\n");
    scanf("%lf",&left_x);
    scanf("%lf",&right_x);
    scanf("%d",&steps);
    double piece = (right_x - left_x) / PROC_NUM;
    double step1 = (right_x - left_x) / steps;
    int every_step = steps / PROC_NUM;




	shmid = shmget (IPC_PRIVATE, sizeof(double), 0666 | IPC_CREAT);
	res = (double*)shmat(shmid, 0, 0);
    sem_t *sem=sem_open("sem", O_CREAT, 0666, 1);
    sem_unlink("sem");

    printf("\n");
    int pid = 0;
    for (i = 0; i < PROC_NUM; i++) {
    	pid = fork();
        if(pid < 0) {
            printf("error forking");
        }
    	if (pid == 0) {
            break;
    	}
    }
    int q;

    if (pid == 0) {
            double temp_sum = 0;
            double left = i * piece + left_x;
            for(q = 0; q < every_step; q++) {
                    temp_sum += step1 * function(A, B, C, left);
                    left += step1;
            }
            sem_wait(sem);
            *res += temp_sum;
            sem_post(sem);
            exit(0);
    }

    for(i = 0; i < PROC_NUM; i++) {
        if (pid > 0) wait(0);
    }



    printf("%lf\n", *res);
    shmdt(res);   
    shmctl(shmid, IPC_RMID, NULL);
    clock_t end = clock();
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;
	printf("\n%s%f\n", "time: ", seconds);

    sem_destroy(sem);
	return 0;

}

double function(double A, double B, double C, double x) {
    return (A*x*x + B*x + C);
}