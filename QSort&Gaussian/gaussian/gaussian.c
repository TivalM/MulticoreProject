#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <omp.h>


#define BILLION 1000000000L
#define bool char

double drand48(), diff;
void srand48();
void *status;
struct timespec start, end;

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



void getUpTriangle(int i){
  p = i + 1;
  while(p < dimN) {
    rate = A[p*dimN+i] / A[i*dimN+i];
    q = i;
    while(q < dimN) {
      A[p*dimN+q] -= rate * A[i*dimN+q];
      q++;
    }
    q = 0;
    while(q < dimN) {
      B[p*dimN+q] -= rate * B[i*dimN+q];
      q++;
    }
    p++;
  } 
}

void swap(double* a, double* b)
{
  double temp = *a;
  *a = *b;
  *b = temp;
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
  myMax = A[i*dimN+i];
  myIdx = i;
  k = i + 1;
  while(k < dimN) {
    if(myMax < A[k*dimN+i]) {
      myMax = A[k*dimN+i];
      myIdx = k;
    }
    k++;
  }
  if(myMax > maxVal) { 
    maxIdx = myIdx;
    maxVal = myMax;
  }
}

void backSubbstitution(){
  i = dimN - 1;
  while(i >= 0){
    p = 0;
    while(p < dimN){
      result[i*dimN+p] = B[i*dimN+p] / A[i*dimN+i];
      p++;
    }
    j = i - 1;
    while(j >= 0) {
      p = 0;
      while(p < dimN){
        B[j*dimN+p] -= A[j*dimN+i] * result[i*dimN+p];
        p++;
      }
      j--;
    }
    i--;
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
  A_test = (double*)malloc(dimN * dimN * sizeof(double));

  result = (double*)malloc(dimN * dimN * sizeof(double));
  RR = (double*)malloc(dimN * dimN * sizeof(double));
  
  
	// populate matrices
  i = 0;
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
  
  /* step2 backsubbstitution */
  backSubbstitution();

	clock_gettime(CLOCK_MONOTONIC,&end);
	diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
  printf("the execution time is %e nanoseconds\n", diff);
  // printf("%d,%d,%e\n", dimN, thr_p, diff);

  return 0;

}