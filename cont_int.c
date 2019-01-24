#include <stdio.h>
#include <time.h>

double function(double A, double B, double C, double x);

int main() {

    clock_t start = clock();

    double left_x, right_x;
    int steps;
    double A = 1;
    double B = 0;
    double C = -4;
    printf("continuous evaluation of integral\nenter left x, right x, amount of steps\n");
    scanf("%lf",&left_x);
    scanf("%lf",&right_x);
    scanf("%d",&steps);



    double del = (right_x - left_x) / steps;
    double res = 0;
    double step = left_x;
    while (step < right_x) {
        res+=function(A, B, C, step)*del;
        step += del;
    }
    printf("%f", res);

    clock_t end = clock();
    float seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("\n%s%f\n", "time: ", seconds);

    return 0;
}

double function(double A, double B, double C, double x) {
    return (A*x*x + B*x + C);
}