#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>

//gcc gaussianpthread.c -o gaussianpthread -lpthread -lm

#define BILLION 1000000000L
#define bool char
#define true 1
#define false 0



struct parameter {
  int start, end;
  int row;
  int thr_id;
};
int i, j, k, r, c, thr_p, rc, step, thr_real;
double *A;
double *B;
double *result;
int maxIndex = 0, dim_n = 0, maxValue = 0;


pthread_mutex_t mutexMax;
pthread_barrier_t myBarrier;
void swap(double* a, double* b)
{
  double temp = *a;
  *a = *b;
  *b = temp;
}


void *GetMaxValue(void *thr_arg) {

  struct parameter *mydata = (struct parameter *) thr_arg;
  int print;
  int p = mydata->start, index = mydata->start;
  double mymax = A[mydata->start*dim_n+mydata->row];
  while(p < mydata->end) {
    if(mymax < A[p*dim_n+mydata->row]) {
      mymax = A[p*dim_n+mydata->row];
      index = p;
    }
    p++;
  }
  pthread_mutex_lock (& mutexMax);
  if(mymax > maxValue) { 
    maxIndex = index;
    maxValue = mymax;
  }
  pthread_mutex_unlock (& mutexMax); 
}


void *getTriangle(void *thr_arg) {

  struct parameter *mydata = (struct parameter *) thr_arg;
  int p = 0, q = 0;
  p = mydata->start;
  while(p < mydata->end){
    double rate = A[p*dim_n+mydata->row] / A[r*dim_n+mydata->row];
    q = r;
    while(q < dim_n) {
      A[p*dim_n+q] -= rate * A[r*dim_n+q];
      q++;
    }
    B[p] -= rate * B[mydata->row]; 
    p++;
  } 
}

void *getDiagonal(void *thr_arg) {

  struct parameter *mydata = (struct parameter *) thr_arg;
  int p = mydata->start;
  while(p < mydata->end) {
    double rate = A[p*dim_n+mydata->row] / A[r*dim_n+mydata->row];
    A[p*dim_n+mydata->row] -= rate * A[r*dim_n+mydata->row];
    B[p] -= rate * B[mydata->row];
    p++;
  } 
}

void *getResult(void *thr_arg) {

  struct parameter *mydata = (struct parameter *) thr_arg;
  int p = mydata->start;
  while(p < mydata->end) {
    result[p] = B[p] / A[p*dim_n+p];
    p++;
  } 
}


int main(int argc, char *argv[]) 
{
	if (argc != 3){
    printf("wrong number of input arguments\n");
    return -1;
	}

  bool flag = false;
	double drand48();
	void srand48();
  void *status;
  struct timespec start, end;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  pthread_mutex_init(&mutexMax, NULL);
  pthread_barrier_init(&myBarrier,NULL,2);
  
  srand48(2);
	dim_n = atoi(argv[1]);
  thr_p = atoi(argv[2]);
  double* A_temp = (double*)malloc(dim_n * dim_n * sizeof(double));
  double* B_temp = (double*)malloc(dim_n * sizeof(double));
  double* result_temp = (double*)malloc(dim_n * sizeof(double));
  pthread_t thread[thr_p];
  
  // set up the data in the heap

  double tmp;
  struct parameter datas[thr_p];
	// populate matrices
  i = 0;
  j = 0;
	while(i < dim_n){
		while(j < dim_n){
			A_temp[i*dim_n+j] = drand48();
      j++;
		}
    B_temp[i] = drand48();
    result_temp[i] = drand48();
    i++;
	}
  A = A_temp;
  B = B_temp;
  result = result_temp;

	clock_gettime(CLOCK_MONOTONIC, &start);
 
  /* step1 triangularization */
  


  /* step2 Diagonalization */
  i = dim_n - 1;
  while(i >= 0){
    
    if(i % thr_p == 0) step = (i) / thr_p;
    else step = (i) / thr_p + 1;
    thr_real = thr_p;
    flag = false;
    k = 0;
    while(k < thr_p){
      datas[k].row = i;
      datas[k].start = step * k;
      datas[k].end = step * (k + 1);
      datas[k].thr_id = k;

      if(datas[k].end >= i) {
        datas[k].end = i;
        thr_real = k + 1;
        flag = true;
      }
      rc = pthread_create(&thread[k], &attr, getDiagonal, (void *) &datas[k]);
      if(flag) break;
      k++;
    }
    k = 0;
    while(k < thr_real) {
      rc = pthread_join(thread[k], &status);
      k++;
    }
    i--;  
  }

  i = 0;
  while(i < dim_n){
  
    // get the max value and index
    maxIndex = i;
    thr_real = thr_p;
    flag = false;
    maxValue = A[i*dim_n+i];
    if((dim_n - i) % thr_p == 0) step = (dim_n - i) / thr_p;
    else step = (dim_n - i) / thr_p + 1;

    k = 0;
    while(k < thr_p){
      datas[k].start = step * k + i;
      datas[k].end = step * (k + 1) + i;
      datas[k].row = i;
      datas[k].thr_id = k;
      if(datas[k].end >= dim_n) {
        datas[k].end = dim_n;
        thr_real = k + 1;
        flag = true;
      }
      rc = pthread_create(&thread[k], &attr, GetMaxValue, (void *) &datas[k]);
      if(flag) break;
      k++;
    }
    k = 0;
    while(k < thr_real){
      rc = pthread_join(thread[k], &status);
      k++;
    }
    
    // swap the line i with the maximum one
    k = i;
    while(k < dim_n) {
      swap(&A[maxIndex*dim_n+k], &A[i*dim_n+k]);
      k++;
    }
    swap(&B[i], &B[maxIndex]);

    // update a(i+1:n,i+1:n) and b(i:n)
    if((dim_n - i - 1) % thr_p == 0) step = (dim_n - i - 1) / thr_p;
    else step = (dim_n - i - 1) / thr_p + 1;
    thr_real = thr_p;
    flag = false;
    k = 0;
    while(k < thr_p){
      datas[k].row = i;
      datas[k].end = step * (k + 1) + i + 1;
      datas[k].thr_id = k;
      datas[k].start = step * k + i + 1;

      
      if(datas[k].end >= dim_n) {
        datas[k].end = dim_n;
        thr_real = k + 1;
        flag = true;
      }
      rc = pthread_create(&thread[k], &attr, getTriangle, (void *) &datas[k]);
      if(flag) break;
      k++;
    }
    k = 0;
    while(k < thr_real) {
      rc = pthread_join(thread[k], &status);
      k++;
    }
    i++;  
  }

  /* step3 Get the result */
  if(dim_n % thr_p == 0) step = (dim_n) / thr_p;
  else step = (dim_n) / thr_p + 1;
  thr_real = thr_p;
  flag = false;
  k = 0;
  while(k < thr_p){
    datas[k].end = step * (k + 1);
    datas[k].thr_id = k;
    datas[k].start = step * k;


    if(datas[k].end >= dim_n) {
      thr_real = k + 1;
      datas[k].end = dim_n;
      flag = true;
    }
    rc = pthread_create(&thread[k], &attr, getResult, (void *) &datas[k]);
    if(flag) break;
    k++;
  }
  k = 0;
  while(k < thr_real) {
    rc = pthread_join(thread[k], &status);
    k++;
  }

  
  pthread_attr_destroy(&attr);
  pthread_barrier_destroy(&myBarrier);
	clock_gettime(CLOCK_MONOTONIC,&end);
	double diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
  printf("the execution time is %e nanoseconds\n", diff);

  
	return 0;
}