#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/time.h>


#define THR_NUM 2


double A = 1;
double B = 0;
double C = -4;
double sum = 0;
pthread_mutex_t mx;

double function(double A, double B, double C, double x);


struct segment {
	double left_x;
	int steps;
	double step1;
};

void *Calculate_Segment(void *segments) {
	struct segment *rangs = segments;
	double left = rangs->left_x;
	int steps = rangs->steps;
	double step1 = rangs->step1;
	free(rangs);
	int i;
	double temp_sum = 0;
	for(i = 0; i < steps; i++) {
		temp_sum += step1 * function(A, B, C, left);
		left += step1;
	}
	pthread_mutex_lock(&mx);
	sum += temp_sum;
	pthread_mutex_unlock(&mx);
	pthread_exit(NULL);
}


int main() {
	if(pthread_mutex_init(&mx, NULL) != 0) {
		printf("error mutex\n");
	}
	printf("parallel evaluation of integral using threads\nenter left x, right x, amount of steps\n");
	double left_x, right_x;
	int steps;
	scanf("%lf %lf %d", &left_x, &right_x, &steps);
	double step1 = (right_x - left_x) / steps;
	int i;
	pthread_t tid[THR_NUM];
	double piece = (right_x - left_x) / THR_NUM;
	int every_step = steps / THR_NUM;
	struct timeval tv_start;
	gettimeofday( &tv_start, NULL);
	for(i = 0; i < THR_NUM; i++) {
		struct segment *segments;
		segments = malloc(sizeof(struct segment));
		segments->left_x = i * piece + left_x;
		segments->steps = every_step;
		segments->step1 = step1;
		pthread_create(&tid[i], NULL, Calculate_Segment, (void*) segments);
	}
	for(i = 0; i < THR_NUM; i++) {
		pthread_join(tid[i], NULL);
	}
	struct timeval tv_end;
	gettimeofday(&tv_end, NULL );
	pthread_mutex_destroy(&mx);
	printf("\n%lf\n", sum);
	printf("Time spent: %lf\n",
		((tv_end.tv_sec * 1000*1000.0 + tv_end.tv_usec) -
			(tv_start.tv_sec * 1000*1000.0 + tv_start.tv_usec)) / 1000/1000
		);
	return(0);
}

double function(double A, double B, double C, double x) {
    return (A*x*x + B*x + C);
}