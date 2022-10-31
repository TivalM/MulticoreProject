/*
steps of compile and unning the code:
gcc gaussianomp.c -o gaussianomp -fopenmp -lm
gaussian dim_n p
*/

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <omp.h>


#define BILLION 1000000000L
#define bool char

double rate;
int maxIdx = 0, myIdx = 0, dimN = 0;
int i, j, k, r, c, p, q, thr_p, rc, step;
double maxVal = 0, myMax = 0;
double* A;
double* A_test;
double* B;
double* B_test;
double* result;
double* RR;
double* tmp;

double drand48(), diff;
void srand48();
void *status;
struct timespec start, end;

void dataInit()
{
  while(i < dimN){
    j = 0;
		while(j < dimN){
			A[i*dimN+j] = rand() % 999;
      A_test[i*dimN+j] = A[i*dimN+j];
      if(i == j) B[i*dimN+j] = 1;
      else B[i*dimN+j] = 0;
      B_test[i*dimN+j] = B[i*dimN+j];
      RR[i*dimN+j] = 0;
      result[i*dimN+j] = rand() % 999;
      j++;
		}
    i++;
	}
}

void swap(double* a, double* b)
{
  double temp = *a;
  *a = *b;
  *b = temp;
}

void *getTriangle(void *thr_arg) {

  struct data_struct *mydata;
  mydata = (struct data_struct *) thr_arg;
  int r = mydata->row;
  int e = mydata->end;
  int s = mydata->start;
  int p = 0, q = 0;
  p = s;
  while(p < e){
    double rate = A[p*dim_n+r] / A[r*dim_n+r];
    q = r;
    while(q < dim_n) {
      A[p*dim_n+q] -= rate * A[r*dim_n+q];
      q++;
    }
    B[p] -= rate * B[r]; 
    p++;
  } 
}

void *getDiagonal(void *thr_arg) {

  struct data_struct *mydata;
  mydata = (struct data_struct *) thr_arg;
  int r = mydata->row;
  int s = mydata->start;
  int e = mydata->end;
  int p = 0;
  p = s;
  while(p < e) {
    double rate = A[p*dim_n+r] / A[r*dim_n+r];
    A[p*dim_n+r] -= rate * A[r*dim_n+r];
    B[p] -= rate * B[r];
    p++;
  } 
}

void *getResult(void *thr_arg) {

  struct data_struct *mydata;
  mydata = (struct data_struct *) thr_arg;
  int s = mydata->start;
  int e = mydata->end;
  int p = s;
  while(p < e) {
    result[p] = B[p] / A[p*dim_n+p];
    p++;
  } 
}

void swapwithMax(int i){
  k = i;
  while(k < dimN) {
    swap(&A[maxIdx*dimN+k], &A[i*dimN+k]);
    k++;
  }
  k = 0;
  while(k < dimN) {
    swap(&B[maxIdx*dimN+k], &B[i*dimN+k]);
    k++;
  }
}

void getMax(int i){
  #pragma omp parallel shared(maxValue, maxIndex) private(mymax, myindex) 
  {
  myMax = A[i*dimN+i];
  myIdx = i;
  #pragma omp for schedule(dynamic)
  for(k = i+1; k < dim_n; k++) {
    if(myMax < A[k*dimN+i]) {
      myMax = A[k*dimN+i];
      myIdx = k;
    }
    k++;
  }
  #pragma omp critical(name){ 
  if(myMax > maxVal) { 
    maxIdx = myIdx;
    maxVal = myMax;
  }}
  }
}

void getUpTriangle(int i){
  #pragma omp parallel for private(rate, p, q) schedule(dynamic)
    for(p = i+1; p < dim_n; p++) {
      rate = A[p*dim_n+i] / A[i*dim_n+i];
      q = i;
      while(q < dim_n) {
        A[p*dim_n+q] -= rate * A[i*dim_n+q];
        q++;
      }
      q = 0;
      while(q < dim_n) {
        B[p*dim_n+q] -= rate * B[i*dim_n+q];
        q++;
      }
    } 
}


int main(int argc, char *argv[]) 
{
	if (argc != 3){
    printf("wrong number of input arguments\n");
    return -1;
	}
  
  srand48(1);
	dimN = atoi(argv[1]);
  thr_p = atoi(argv[2]);  
  // set up the data in the heap
  B = (double*)malloc(dimN * dimN * sizeof(double));
  B_test = (double*)malloc(dimN * dimN * sizeof(double));

  A = (double*)malloc(dimN * dimN * sizeof(double));
  RR = (double*)malloc(dimN * dimN * sizeof(double));

  A_test = (double*)malloc(dimN * dimN * sizeof(double));

  result = (double*)malloc(dimN * dimN * sizeof(double));  
  
	// populate matrices
  i = 0;
	dataInit();

	clock_gettime(CLOCK_MONOTONIC, &start);
 
  /* step1 triangularization */
  i = 0;
  while(i < dimN){
  
    // get the max value and index
    maxVal = A[i*dimN+i];
    maxIdx = i;
    
    getMax(i);
    
    // swap the line i with the maximum one
    swapwithMax(i);
        
    // update a(i+1:n,i+1:n) and b(i+1:n, :)
    getUpTriangle(i);
    i++;
  }
  

	clock_gettime(CLOCK_MONOTONIC,&end);
	diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
  printf("the execution time is %e nanoseconds\n", diff);
  return 0;

}