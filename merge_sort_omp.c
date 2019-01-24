#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <malloc.h>
#include <math.h>


#define CORE_AMOUNT 2
#define AMOUNT 1000000
#define RANGE 1000000

void randomize(int *array);
void make_same(int *array, int *array_tmp);
void print(int *array);
double run_sequentially(int *array);
double run_parallel(int *array);
int merge_sort(int *array, int left, int right);
int merge_sort_omp(int *array, int left, int right);

int main() {

  double time_seq, time_par;
  int *array_seq = (int*)malloc(AMOUNT*sizeof(int));
  int *array_par = (int*)malloc(AMOUNT*sizeof(int));


  randomize(array_seq);
  make_same(array_par, array_seq);

  printf("the first array is randomized:\n");
  //print(array_seq);
  printf("the second array is the same as the first:\n");
  //print(array_par);

  time_seq = run_sequentially(array_seq);
  time_par = run_parallel(array_par);

  printf("%s\n%lf\n", "time for sequential sorting:", time_seq);
  printf("%s\n%lf\n", "time for parallel sorting:", time_par);
  printf("%s\n%lf\n", "the ratio of the time:", time_seq/time_par);

  free(array_seq);
  free(array_par);

  return 0;
}

double run_sequentially(int *array){

  double temp_time = omp_get_wtime();
  merge_sort(array, 0, AMOUNT-1);

  printf("sequential sorting:\n");
  //print(array);

  return omp_get_wtime() - temp_time;

}


double run_parallel(int *array){

  double t=omp_get_wtime();
  omp_set_nested(1);
  omp_set_max_active_levels((int)log((double)CORE_AMOUNT));
  merge_sort_omp(array, 0, AMOUNT - 1);

  printf("parallel sorting using omp:\n");
  //print(array);
  
  return omp_get_wtime() - t;
}

int merge_sort(int *array, int left, int right) {

  int segm = right - left;
  int half = left + segm/2;

  if(segm > 1){
    merge_sort(array, left, half);
    merge_sort(array, half+1, right);
  }

  if (segm > 0){
    int tmp_left = left;
    int tmp_right = half + 1;
    int *tmp = (int*)malloc((segm + 1) *sizeof(int));
    int i = 0;
    while(i <= segm) {
      if(array[tmp_left] < array[tmp_right]) {
        if(tmp_left < half) {
          tmp[i++] = array[tmp_left++];
        }
        else {
          tmp[i++] = array[tmp_left];
          while(tmp_right <= right) { 
            tmp[i++] = array[tmp_right++];
          }
        }
      }
      else {
        if(tmp_right < right) {
          tmp[i++] = array[tmp_right++];
        }
        else {
          tmp[i++] = array[tmp_right];
          while(tmp_left <= half) {
            tmp[i++] = array[tmp_left++];
          }
          break;
        }
      }
    }
    for(int j = 0; j <= segm; j++) {
    	array[left+j] = tmp[j];
    }
    free(tmp);
  }

  return 0;
}


int merge_sort_omp(int *array, int left, int right) {

  int segm = right - left;
  int half = left + segm/2;

  if(segm > 1) {

    if(omp_get_active_level() >= omp_get_max_active_levels()) {
      merge_sort_omp(array, left, half);
      merge_sort_omp(array, half+1, right);
    }
    else {

      #pragma omp parallel num_threads(2)
      {
        int i = omp_get_thread_num();
        if (i == 0) {
          merge_sort_omp(array, left, half);
        } 
        else {
          merge_sort_omp(array, half+1, right);
        }
      }
    }
  }

  if (segm > 0){

    int tmp_left = left;
    int tmp_right = half + 1;
    int *tmp = (int*)malloc((segm+1)*sizeof(int));
    int i = 0;

    while(i <= segm){
      if(array[tmp_left] < array[tmp_right]) {
        if(tmp_left < half)  {  
          tmp[i++] = array[tmp_left++];
        }
        else {
          tmp[i++] = array[tmp_left];
          while(tmp_right <= right) {
            tmp[i++] = array[tmp_right++];
          }
        }
      }
      else {
        if(tmp_right < right) {
          tmp[i++] = array[tmp_right++];
        }
        else {
          tmp[i++] = array[tmp_right];
          while(tmp_left <= half) {
            tmp[i++] = array[tmp_left++];
          }
          break;
        }
      }
    }
    for(int j = 0; j <= segm; j++) {
      array[left+j] = tmp[j];
    }
    free(tmp);
  }

  return 0;
}



void randomize(int *array){
  for(int i = 0; i < AMOUNT; i++)  
    array[i] = rand() % RANGE - RANGE/2;
}

void make_same(int *array, int *array_tmp) {
  for (int i = 0; i < AMOUNT; i++) {
    array[i] = array_tmp[i];
  }
}
 
void print(int *array){
  for(int i = 0; i < AMOUNT; i++) 
    printf("%i ", array[i]);
  printf("\n");
}
